package net.labymedia.ultralight.containers;

import net.labymedia.ultralight.javascript.*;
import net.labymedia.ultralight.utils.JavascriptUtils;

import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

// TODO: Rename this class.
// TODO: Improve error handling.
public final class CustomJavascriptClassDefinition {
    private final JavascriptClassDefinition staticDefinition;

    private final Map<String, Field> fieldLookupTable = new HashMap<>();
    private final Map<String, JavascriptFunction> methodLookupTable = new HashMap<>();

    private CustomJavascriptClassDefinition(final String name) {
        staticDefinition = new JavascriptClassDefinition().name(name);
    }

    private void initializeCallbacks() {
        staticDefinition.onGetProperty(this::staticOnGetProperty);
        staticDefinition.onSetProperty(this::staticOnSetProperty);
    }

    private JavascriptValue staticOnGetProperty(JavascriptContext context,
                                                JavascriptObject javascriptObject,
                                                String propertyName) {
        Field field = fieldLookupTable.get(propertyName);

        if (field == null) {
            JavascriptFunction function = methodLookupTable.get(propertyName);
            if (function == null) return null;
            return context.makeObject(function.functionClass(), function.methodHandle());
        }

        try {
            return JavascriptUtils.toJavascriptValue(context, field.get(null));
        } catch (IllegalAccessException exception) {
            exception.printStackTrace();
            return null;
        }
    }

    private boolean staticOnSetProperty(JavascriptContext context,
                                        JavascriptObject javascriptObject,
                                        String propertyName,
                                        JavascriptValue value) {
        Field field = fieldLookupTable.get(propertyName);

        if (field == null) return false;

        try {
            field.set(null, JavascriptUtils.fromJavascriptValue(value).value());
            return true;
        } catch (IllegalAccessException exception) {
            exception.printStackTrace();
            return false;
        }
    }

    private void populateMethodLookupTable(final Method[] methods) throws IllegalAccessException {
        MethodHandles.Lookup lookup = MethodHandles.publicLookup();

        for (Method method : methods) {
            MethodHandle methodHandle = lookup.unreflect(method);

            JavascriptClassDefinition functionDefinition = new JavascriptClassDefinition()
                .name(method.getName())
                .attributes(JavascriptClassAttributes.NO_AUTOMATIC_PROTOTYPE);

            functionDefinition.onCallAsFunction((context, function, thisObject, arguments) -> {
                try {
                    Object result = ((MethodHandle) function.getPrivate()).invoke();
                    if (result != null) return JavascriptUtils.toJavascriptValue(context, result);
                    return context.makeNull();
                } catch (Throwable throwable) {
                    throwable.printStackTrace();
                    return null;
                }
            });

            methodLookupTable.put(method.getName(), new JavascriptFunction(functionDefinition.bake(), methodHandle));
        }
    }

    private void populateFieldLookupTable(final Field[] fields) {
        for (Field field : fields) {
            fieldLookupTable.put(field.getName(), field);
        }
    }

    public JavascriptClassDefinition staticDefinition() {
        return staticDefinition;
    }

    public static CustomJavascriptClassDefinition from(final Object object) throws IllegalAccessException {
        Class<?> clazz = object.getClass();

        CustomJavascriptClassDefinition definition = new CustomJavascriptClassDefinition(clazz.getName());
        definition.populateMethodLookupTable(clazz.getMethods());
        definition.populateFieldLookupTable(clazz.getFields());
        definition.initializeCallbacks();

        return definition;
    }
}
