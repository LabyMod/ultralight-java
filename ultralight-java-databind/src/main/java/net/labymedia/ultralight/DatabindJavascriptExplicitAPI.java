package net.labymedia.ultralight;

import net.labymedia.ultralight.javascript.*;
import net.labymedia.ultralight.utils.JavascriptConversionUtils;

import java.util.ArrayList;
import java.util.List;

public final class DatabindJavascriptExplicitAPI {
    private final JavascriptClassDefinition definition;

    private final JavascriptConversionUtils conversionUtils;

    private DatabindJavascriptExplicitAPI(JavascriptConversionUtils conversionUtils, String name) {
        this.definition = new JavascriptClassDefinition()
            .name(name)
            .attributes(JavascriptClassAttributes.NO_AUTOMATIC_PROTOTYPE);

        this.conversionUtils = conversionUtils;
    }

    private void registerCallbacks() {
        definition.onCallAsFunction(this::onCallAsFunction);
    }

    private JavascriptValue onCallAsFunction(JavascriptContext context, JavascriptObject function, JavascriptObject thisObject, JavascriptValue[] arguments) {
        Data privateData = (Data) function.getPrivate();
        List<Class<?>> parameterTypes = new ArrayList<>();

        for (JavascriptValue value : arguments) {
            Object object = conversionUtils.fromJavascript(value);
            System.out.println(object);
            parameterTypes.add(object == null ? null : object.getClass());
        }

        return context.makeObject(privateData.methodHandler(), DatabindJavascriptMethodHandler.Data.builder()
            .instance(privateData.instance())
            .parameterTypes(parameterTypes.toArray(new Class[0]))
            .build());
    }

    public JavascriptClass bake() {
        return definition.bake();
    }

    static DatabindJavascriptExplicitAPI create(JavascriptConversionUtils conversionUtils, String name) {
        DatabindJavascriptExplicitAPI javascriptClass = new DatabindJavascriptExplicitAPI(conversionUtils, name);
        javascriptClass.registerCallbacks();
        return javascriptClass;
    }

    public static class Data {
        private Object instance;
        private JavascriptClass methodHandler;

        private Data() { }

        private Object instance() {
            return instance;
        }

        public JavascriptClass methodHandler() {
            return methodHandler;
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

            public Builder methodHandler(JavascriptClass methodHandler) {
                data.methodHandler = methodHandler;
                return this;
            }

            public Data build() {
                return data;
            }
        }
    }
}
