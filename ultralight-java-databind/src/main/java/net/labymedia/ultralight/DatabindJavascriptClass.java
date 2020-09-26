package net.labymedia.ultralight;

import net.labymedia.ultralight.javascript.*;
import net.labymedia.ultralight.javascript.interop.JavascriptInteropException;
import net.labymedia.ultralight.utils.JavascriptConversionUtils;
import net.labymedia.ultralight.utils.MethodChooser;
import net.labymedia.ultralight.utils.VarArgsUtils;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.*;

public final class DatabindJavascriptClass {
    private final JavascriptClassDefinition definition;

    private final DatabindConfiguration configuration;
    private final JavascriptConversionUtils conversionUtils;
    private final MethodChooser methodChooser;

    private final Set<Constructor<?>> constructors = new HashSet<>();
    private final Map<String, Set<Method>> methods = new HashMap<>();
    private final Map<String, Field> fields = new HashMap<>();

    private DatabindJavascriptClass(DatabindConfiguration configuration, JavascriptConversionUtils conversionUtils, String name) {
        this.definition = new JavascriptClassDefinition()
            .name(name)
            .attributes(JavascriptClassAttributes.NO_AUTOMATIC_PROTOTYPE);

        this.configuration = configuration;
        this.conversionUtils = conversionUtils;
        this.methodChooser = configuration.methodChooser();
    }

    private void registerCallbacks() {
        definition.onCallAsConstructor(this::onCallAsConstructor);
        definition.onGetProperty(this::onGetProperty);
        definition.onSetProperty(this::onSetProperty);
    }

    private void addConstructors(Constructor<?>... constructors) {
        this.constructors.addAll(Arrays.asList(constructors));
    }

    private void addMethods(Method... methods) {
        for (Method method : methods) {
            String name = method.getName();

            if (this.methods.containsKey(name)) {
                this.methods.get(name).add(method);
                continue;
            }

            Set<Method> methodSet = new HashSet<>();
            methodSet.add(method);

            this.methods.put(name, methodSet);
        }
    }

    private void addFields(Field... fields) {
        for (Field field : fields) {
            this.fields.put(field.getName(), field);
        }
    }

    private JavascriptObject onCallAsConstructor(JavascriptContext context, JavascriptObject constructor, JavascriptValue[] arguments) throws JavascriptInteropException {
        List<Class<?>> parameterTypes = new ArrayList<>();
        List<Object> parameters = new ArrayList<>();

        for (JavascriptValue value : arguments) {
            Object object = conversionUtils.fromJavascript(value);
            parameterTypes.add(object == null ? null : object.getClass());
            parameters.add(object);
        }

        Constructor<?> method = (Constructor<?>) methodChooser.choose(constructors, parameterTypes.toArray(new Class[0]));

        if (method == null) {
            throw new JavascriptInteropException("Unable to determine method");
        }

        if (method.isVarArgs()) {
            parameters = VarArgsUtils.toVarArgs(parameters, method);
        }

        try {
            return context.makeObject(bake(), Data.builder()
                .instance(method.newInstance(parameters.toArray()))
                .build());
        } catch (IllegalAccessException exception) {
            throw new JavascriptInteropException("Unable to access constructor: " + method.getName(), exception);
        } catch (InvocationTargetException exception) {
            throw new JavascriptInteropException("Constructor threw an exception", exception);
        } catch (InstantiationException exception) {
            throw new JavascriptInteropException("Unable to create instance", exception);
        }
    }

    private JavascriptValue onGetProperty(JavascriptContext context, JavascriptObject object, String propertyName) throws JavascriptInteropException {
        Data privateData = (Data) object.getPrivate();
        Field field = fields.get(propertyName);

        if (field != null) {
            try {
                return conversionUtils.toJavascript(context, field.get(privateData.instance()));
            } catch (IllegalAccessException exception) {
                throw new JavascriptInteropException("Unable to access field: " + field.getName(), exception);
            }
        }

        Set<Method> methodSet = methods.get(propertyName);

        if (methodSet == null) {
            return context.makeUndefined();
        }

        return context.makeObject(DatabindJavascriptMethodHandler.create(configuration, conversionUtils, methodSet, propertyName).bake(), DatabindJavascriptMethodHandler.Data.builder()
            .instance(privateData.instance())
            .build());
    }

    private boolean onSetProperty(JavascriptContext context, JavascriptObject object, String propertyName, JavascriptValue value) throws JavascriptInteropException {
        Field field = fields.get(propertyName);

        if (field != null) {
            try {
                field.set(object, conversionUtils.fromJavascript(value));
            } catch (IllegalAccessException exception) {
                throw new JavascriptInteropException("Unable to access field: " + field.getName(), exception);
            }
        }

        return false;
    }

    public JavascriptClass bake() {
        return definition.bake();
    }

    static DatabindJavascriptClass create(DatabindConfiguration configuration, JavascriptConversionUtils conversionUtils, String name, Class<?> javaClass) {
        DatabindJavascriptClass javascriptClass = new DatabindJavascriptClass(configuration, conversionUtils, name);

        javascriptClass.registerCallbacks();
        javascriptClass.addConstructors(javaClass.getDeclaredConstructors());
        javascriptClass.addMethods(javaClass.getDeclaredMethods());
        javascriptClass.addFields(javaClass.getDeclaredFields());

        return javascriptClass;
    }

    public static class Data {
        private Object instance;
        private Class<?> javaClass;

        private Data() { }

        public Object instance() {
            return instance;
        }

        public Class<?> javaClass() {
            return javaClass;
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

            public Builder javaClass(Class<?> javaClass) {
                data.javaClass = javaClass;
                return this;
            }

            public Data build() {
                return data;
            }
        }
    }
}
