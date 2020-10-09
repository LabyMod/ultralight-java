package net.labymedia.ultralight;

import net.labymedia.ultralight.api.InjectJavascriptContext;
import net.labymedia.ultralight.javascript.*;
import net.labymedia.ultralight.javascript.interop.JavascriptInteropException;
import net.labymedia.ultralight.utils.JavascriptConversionUtils;
import net.labymedia.ultralight.utils.MethodChooser;
import net.labymedia.ultralight.utils.VarArgsUtils;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;

public final class DatabindJavascriptMethodHandler {
    private final JavascriptClassDefinition definition;
    private final String name;

    private final MethodChooser methodChooser;
    private final JavascriptConversionUtils conversionUtils;

    private final Set<Method> methodSet;

    private DatabindJavascriptMethodHandler(DatabindConfiguration configuration, JavascriptConversionUtils conversionUtils, Set<Method> methodSet, String name) {
        this.definition = new JavascriptClassDefinition()
            .name(name)
            .attributes(JavascriptClassAttributes.NO_AUTOMATIC_PROTOTYPE);

        this.name = name;
        this.methodChooser = configuration.methodChooser();
        this.conversionUtils = conversionUtils;
        this.methodSet = methodSet;
    }

    private void registerCallbacks() {
        definition.onCallAsFunction(this::onCallAsFunction);
        definition.onGetProperty(this::onGetProperty);
    }

    private JavascriptValue onCallAsFunction(JavascriptContext context, JavascriptObject function, JavascriptObject thisObject, JavascriptValue[] arguments) throws JavascriptInteropException {
        Data privateData = (Data) function.getPrivate();
        Method method;

        if (privateData.parameterTypes() == null) {
            method = (Method) methodChooser.choose(methodSet, arguments);
        } else {
            method = (Method) methodChooser.choose(methodSet, privateData.parameterTypes());
        }

        List<Object> parameters = new ArrayList<>();

        if (method == null) {
            throw new JavascriptInteropException("Unable to determine method");
        }

        for (int i = 0; i < arguments.length; i++) {
            Object object = conversionUtils.fromJavascript(arguments[i], method.isAnnotationPresent(InjectJavascriptContext.class) ? method.getParameterTypes()[i + 1] : method.getParameterTypes()[i]);
            parameters.add(object);
        }

        if (method.isAnnotationPresent(InjectJavascriptContext.class)) {
            parameters.add(0, context);
        }

        if (method.isVarArgs()) {
            parameters = VarArgsUtils.toVarArgs(parameters, method);
        }

        try {
            return conversionUtils.toJavascript(context, method.invoke(privateData.instance(), parameters.toArray()));
        } catch (IllegalAccessException exception) {
            throw new JavascriptInteropException("Unable to access method: " + method.getName(), exception);
        } catch (InvocationTargetException exception) {
            throw new JavascriptInteropException(method.getName() + " threw an exception", exception);
        }
    }

    private JavascriptValue onGetProperty(JavascriptContext context, JavascriptObject object, String propertyName) {
        if (!propertyName.equals("signature")) return context.makeUndefined();
        Data privateData = (Data) object.getPrivate();

        return context.makeObject(DatabindJavascriptExplicitAPI.create(conversionUtils, name).bake(), DatabindJavascriptExplicitAPI.Data.builder()
            .instance(privateData.instance())
            .methodHandler(bake())
            .build());
    }

    public JavascriptClass bake() {
        return definition.bake();
    }

    static DatabindJavascriptMethodHandler create(DatabindConfiguration configuration, JavascriptConversionUtils conversionUtils, Set<Method> methodSet, String name) {
        DatabindJavascriptMethodHandler javascriptClass = new DatabindJavascriptMethodHandler(configuration, conversionUtils, methodSet, name);
        javascriptClass.registerCallbacks();
        return javascriptClass;
    }

    public static class Data {
        private Object instance;
        private Class<?>[] parameterTypes;

        private Data() { }

        public Object instance() {
            return instance;
        }

        public Class<?>[] parameterTypes() {
            return parameterTypes;
        }

        public static Builder builder() {
            return new Builder();
        }

        public static class Builder {
            private final Data data = new Data();

            private Builder() { }

            public Builder instance(Object instance) {
                data.instance = instance;
                return this;
            }

            public Builder parameterTypes(Class<?>... parameterTypes) {
                data.parameterTypes = parameterTypes;
                return this;
            }

            public Data build() {
                return data;
            }
        }
    }
}
