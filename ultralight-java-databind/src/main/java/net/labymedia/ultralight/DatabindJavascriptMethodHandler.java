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
        CallData<Method> callData;

        if (privateData.parameterTypes() == null) {
            callData = methodChooser.choose(methodSet, arguments);
        } else {
            callData = methodChooser.choose(methodSet, privateData.parameterTypes(), arguments);
        }

        Method method = callData.getTarget();
        Parameter[] methodParameters = method.getParameters();
        List<Object> parameters = new ArrayList<>();

        boolean injectContext = method.isAnnotationPresent(InjectJavascriptContext.class);

        for(int i = 0; i < methodParameters.length; i++) {
            if(i == 0 && injectContext) {
                parameters.add(context);
            } else if(i == methodParameters.length - 1 && method.isVarArgs()) {
                switch (callData.getVarArgsType()) {
                    case NONE:
                    case EMPTY:
                        break;

                    case COMPACT:
                        int varArgsCount = arguments.length - i;
                        Class<?> targetType = methodParameters[i].getType().getComponentType();

                        Object args = Array.newInstance(targetType, varArgsCount);

                        for(int x = 0; x < varArgsCount; x++) {
                            Array.set(args, x, conversionUtils.fromJavascript(arguments[(i + x) - (injectContext ? 1 :0)], targetType));
                        }

                        parameters.add(args);
                        break;

                    case PASS_THROUGH:
                        parameters.add(conversionUtils.fromJavascript(arguments[i - (injectContext ? 1 :0)], methodParameters[i].getType()));
                        break;
                }
            } else {
                parameters.add(conversionUtils.fromJavascript(arguments[i - (injectContext ? 1 :0)], methodParameters[i].getType()));
            }
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
