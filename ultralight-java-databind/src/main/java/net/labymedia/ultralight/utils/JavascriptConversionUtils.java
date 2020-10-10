package net.labymedia.ultralight.utils;

import net.labymedia.ultralight.Databind;
import net.labymedia.ultralight.DatabindJavascriptClass;
import net.labymedia.ultralight.javascript.*;

import java.lang.reflect.Array;
import java.util.Date;
import java.util.Objects;

public final class JavascriptConversionUtils {
    private final Databind databind;

    public JavascriptConversionUtils(Databind databind) {
        this.databind = databind;
    }

    public JavascriptValue toJavascript(JavascriptContext context, Object object) {
        if (object == null || object == JavascriptType.NULL) return context.makeNull();
        if (object == JavascriptType.UNDEFINED) return context.makeUndefined();
        Class<?> javaClass = object.getClass();

        if (javaClass == Boolean.class) return context.makeBoolean((boolean) object);

        if (javaClass == Byte.class
         || javaClass == Short.class
         || javaClass == Integer.class
         || javaClass == Long.class
         || javaClass == Float.class
         || javaClass == Double.class) return context.makeNumber(((Number) object).doubleValue());

        if (javaClass == String.class) return context.makeString((String) object);

        if (javaClass.isArray()) {
            Object[] array = (Object[]) object;
            JavascriptValue[] values = new JavascriptValue[array.length];

            for (int i = 0; i < array.length; i++) {
                values[i] = toJavascript(context, array[i]);
            }

            return context.makeArray(values);
        }

        if (object instanceof Date) {
            return context.makeDate(
                context.makeNumber(((Date) object).getTime())
            );
        }

        if (object instanceof JavascriptValue) return (JavascriptValue) object;

        if (javaClass == Class.class) {
            return context.makeObject(databind.toJavascript(javaClass, true), DatabindJavascriptClass.Data.builder()
                .javaClass(javaClass)
                .build());
        }

        if (javaClass == JavascriptClass.class) {
            return context.makeObject((JavascriptClass) object, DatabindJavascriptClass.Data.builder()
                .build());
        }

        return context.makeObject(databind.toJavascript(javaClass), DatabindJavascriptClass.Data.builder()
            .javaClass(javaClass)
            .instance(object)
            .build());
    }

    public Object fromJavascript(JavascriptValue value, Class<?> type) {
        if (value.isNull() || value.isUndefined()) return null;

        if (value.isBoolean()) return value.toBoolean();
        if (value.isNumber()) {
            Number number = value.toNumber();

            if (type == byte.class) return number.byteValue();
            if (type == short.class) return number.shortValue();
            if (type == int.class) return number.intValue();
            if (type == long.class) return number.longValue();
            if (type == float.class) return number.floatValue();
            if (type == double.class) return number.doubleValue();

            throw new IllegalArgumentException("Javascript number could not be converted to a Java number");
        }

        if (value.isString()) return value.toString();

        if (value.isObject()) {
            if (value.isDate()) {
                long millis = (long) value.toObject().getProperty("getTime").toObject().callAsFunction(value.toObject()).toNumber();
                return new Date(millis);
            } else if (value.isArray()) {
                JavascriptObject object = value.toObject();
                int size = (int) object.getProperty("length").toNumber();

                Object objects = Array.newInstance(type.getComponentType(), size);

                for (int i = 0; i < size; i++) {
                    Array.set(objects, i, fromJavascript(object.getPropertyAtIndex(i), type));
                }

                return objects;
            }

            DatabindJavascriptClass.Data privateData = Objects.requireNonNull((DatabindJavascriptClass.Data) value.toObject().getPrivate(), "Cannot convert arbitrary JavaScript object to Java object");

            if (privateData.instance() == null) {
                return privateData.javaClass();
            } else {
                return privateData.instance();
            }
        }

        throw new IllegalArgumentException("Javascript value could not be converted to a Java object");
    }

    public static Class<?> determineType(JavascriptValue value) {
        if (value.isNull() || value.isUndefined()) return Object.class;

        if (value.isBoolean()) return boolean.class;
        if (value.isNumber()) return Number.class;

        if (value.isString()) return String.class;

        if (value.isObject()) {
            JavascriptObject object = value.toObject();

            if (value.isDate()) {
                return Date.class;
            } else if (value.isArray()) {
                int size = (int) object.getProperty("length").toNumber();

                JavascriptValue[] values = new JavascriptValue[size];

                for (int i = 0; i < size; i++) {
                    values[i] = object.getPropertyAtIndex(i);
                }

                return Array.newInstance(calculateGCC(values), 0).getClass();
            }

            if(object.getPrivate() == null) {
                return JavascriptObject.class;
            }

            DatabindJavascriptClass.Data privateData = (DatabindJavascriptClass.Data) object.getPrivate();

            if (privateData.instance() == null) {
                return privateData.javaClass();
            } else {
                return privateData.instance().getClass();
            }
        }

        throw new AssertionError("UNREACHABLE: Could not convert JavascriptValue to any java class");
    }

    private static Class<?> calculateGCC(JavascriptValue... values) {
        Class<?> gcc = null;

        for (JavascriptValue value : values) {
            Class<?> type = determineType(value);

            if (gcc == null) {
                gcc = type;
            } else if (type.isAssignableFrom(gcc)) {
                gcc = type;
            }

            for (Class<?> javaInterface : type.getInterfaces()) {
                if (javaInterface.isAssignableFrom(gcc)) {
                    gcc = type;
                }
            }
        }

        return gcc;
    }
}
