package net.labymedia.ultralight;

import net.labymedia.ultralight.api.InjectJavascriptContext;
import net.labymedia.ultralight.call.CallData;
import net.labymedia.ultralight.call.MethodChooser;
import net.labymedia.ultralight.javascript.*;
import net.labymedia.ultralight.javascript.interop.JavascriptInteropException;
import net.labymedia.ultralight.utils.JavascriptConversionUtils;

import java.lang.reflect.Array;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Parameter;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

/**
 * Method handler for calls inbound from Javascript.
 */
public final class DatabindJavascriptMethodHandler {
    private final JavascriptClassDefinition definition;
    private final String name;

    private final MethodChooser methodChooser;
    private final JavascriptConversionUtils conversionUtils;

    private final Set<Method> methodSet;

    /**
     * Constructs a new {@link DatabindJavascriptMethodHandler}.
     *
     * @param configuration   The configuration to use
     * @param conversionUtils The conversion utilities to use for converting objects
     * @param methodSet       The methods which can be invoked by this handler
     * @param name            The name of this handler class
     */
    private DatabindJavascriptMethodHandler(
            DatabindConfiguration configuration,
            JavascriptConversionUtils conversionUtils,
            Set<Method> methodSet,
            String name
    ) {
        this.definition = new JavascriptClassDefinition()
                .name(name)
                .attributes(JavascriptClassAttributes.NO_AUTOMATIC_PROTOTYPE);

        this.name = name;
        this.methodChooser = configuration.methodChooser();
        this.conversionUtils = conversionUtils;
        this.methodSet = methodSet;
    }

    /**
     * Registers the callbacks on the definition.
     */
    private void registerCallbacks() {
        definition.onCallAsFunction(this::onCallAsFunction);
        definition.onGetProperty(this::onGetProperty);
    }

    /**
     * Called by Javascript when a request is issued to invoke a function matching this handler.
     *
     * @param context    The Javascript context the function is being invoked int
     * @param function   The function being called
     * @param thisObject The this parameter of the call
     * @param arguments  The arguments of the call
     * @return The result of the call
     * @throws JavascriptInteropException If invoking the method fails
     */
    private JavascriptValue onCallAsFunction(
            JavascriptContext context,
            JavascriptObject function,
            JavascriptObject thisObject,
            JavascriptValue[] arguments
    ) throws JavascriptInteropException {
        Data privateData = (Data) function.getPrivate();
        CallData<Method> callData;

        if (privateData.parameterTypes() == null) {
            // Implicit types
            callData = methodChooser.choose(methodSet, arguments);
        } else {
            // Explicit types
            callData = methodChooser.choose(methodSet, privateData.parameterTypes(), arguments);
        }

        // Prepare the call
        Method method = callData.getTarget();
        List<Object> parameters = callData.constructArguments(
                context,
                conversionUtils,
                arguments
        );

        try {
            // Invoke method with constructed arguments
            return conversionUtils.toJavascript(context, method.invoke(privateData.instance(), parameters.toArray()));
        } catch (IllegalAccessException exception) {
            throw new JavascriptInteropException("Unable to access method: " + method.getName(), exception);
        } catch (InvocationTargetException exception) {
            throw new JavascriptInteropException(method.getName() + " threw an exception", exception);
        }
    }

    /**
     * Called by Javascript when a property is requested on the function.
     *
     * @param context      The context the property is requested in
     * @param object       The object the property is requested from
     * @param propertyName The name of the requested property
     * @return The found property, or {@code null}, if the property could not be found
     */
    private JavascriptValue onGetProperty(JavascriptContext context, JavascriptObject object, String propertyName) {
        if (!propertyName.equals("signature")) {
            return null;
        }

        Data privateData = (Data) object.getPrivate();

        // Create the explicit API interface
        return context.makeObject(DatabindJavascriptExplicitAPI.create(conversionUtils, name).bake(),
                new DatabindJavascriptExplicitAPI.Data(privateData.instance, bake()));
    }

    /**
     * Bakes the class definition.
     *
     * @return The baked definition
     */
    public JavascriptClass bake() {
        return definition.bake();
    }

    /**
     * Creates a new {@link DatabindJavascriptMethodHandler}.
     *
     * @param configuration   The configuration to use
     * @param conversionUtils The conversion utilities to user for converting objects
     * @param methodSet       The sets of methods invocable by this handler
     * @param name            The name of this handler class
     * @return The created handler
     */
    static DatabindJavascriptMethodHandler create(
            DatabindConfiguration configuration,
            JavascriptConversionUtils conversionUtils,
            Set<Method> methodSet,
            String name
    ) {
        DatabindJavascriptMethodHandler javascriptClass = new DatabindJavascriptMethodHandler(configuration, conversionUtils, methodSet, name);
        javascriptClass.registerCallbacks();
        return javascriptClass;
    }

    /**
     * Data of a not yet parameter bound call.
     */
    public static class Data {
        private final Object instance;
        private final Class<?>[] parameterTypes;

        /**
         * Constructs a new {@link Data} instance for a not yet bound call.
         *
         * @param instance       The instance this call will be invoked on
         * @param parameterTypes The requested parameter types, or {@code null}, if not known
         */
        public Data(Object instance, Class<?>[] parameterTypes) {
            this.instance = instance;
            this.parameterTypes = parameterTypes;
        }

        /**
         * Retrieves the instance this call will be invoked on.
         *
         * @return The instance this call will be invoked on
         */
        public Object instance() {
            return instance;
        }

        /**
         * Retrieves the type of the parameters this call will be invoked with.
         *
         * @return The type of the parameters this will be invoked with, or {@code null}, if not known
         */
        public Class<?>[] parameterTypes() {
            return parameterTypes;
        }
    }
}
