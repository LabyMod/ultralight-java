package net.labymedia.ultralight.utils;

import net.labymedia.ultralight.containers.GenericValueContainer;
import net.labymedia.ultralight.javascript.JavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptValue;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

public final class JavascriptUtils {
    private static DateFormat JAVASCRIPT_DATE_FORMAT = new SimpleDateFormat("EE MMM d y H:m:s 'GMT'Z (zz)");

    private JavascriptUtils() { }

    public String generateDescriptor(final Class<?>... parameters) {
        StringBuilder builder = new StringBuilder();

        for (Class<?> parameter : parameters) {
            builder.append(generateDescriptor(parameter));
        }

        return builder.toString();
    }

    public String generateDescriptor(final Class<?> parameter) {
        if (parameter.isPrimitive()) {
            if (parameter == short.class
                    || parameter == int.class
                    || parameter == long.class
                    || parameter == float.class
                    || parameter == double.class
                    || parameter == byte.class) return "I";
        }
        return "";
    }

    // TODO: Support JS Date & Array
    public static JavascriptValue toJavascriptValue(final JavascriptContext context, final Object javaObject) {
        Class<?> javaClass = javaObject.getClass();

        if (javaClass == Short.class
                || javaClass == Integer.class
                || javaClass == Long.class
                || javaClass == Float.class
                || javaClass == Double.class
                || javaClass == Byte.class) return context.makeNumber((double) javaObject);
        if (javaClass == Character.class) return context.makeString(String.valueOf((char) javaObject));
        if (javaClass == Boolean.class) return context.makeBoolean((boolean) javaObject);
        if (javaClass == String.class) return context.makeString((String) javaObject);
        if (javaClass == Date.class) {
            Date date = (Date) javaObject;

            return context.makeDate(
                context.makeNumber(date.getYear() + 1900),
                context.makeNumber(date.getMonth()),
                context.makeNumber(date.getDate()),
                context.makeNumber(date.getHours()),
                context.makeNumber(date.getMinutes()),
                context.makeNumber(date.getSeconds())
            );
        }

        throw new IllegalArgumentException("Java object could not be contained in a javascript value");
    }

    @SuppressWarnings("unchecked")
    public static <T> GenericValueContainer<T> fromJavascriptValue(final JavascriptValue value) {
        if (value.isObject()) {
            if (value.isDate()) {
                try {
                    return (GenericValueContainer<T>) new GenericValueContainer<>(JAVASCRIPT_DATE_FORMAT.parse(value.toString()));
                } catch (ParseException exception) {
                    exception.printStackTrace();
                }
            }
        } else {
            if (value.isNumber()) return (GenericValueContainer<T>) new GenericValueContainer<>(value.toNumber());
            if (value.isBoolean()) return (GenericValueContainer<T>) new GenericValueContainer<>(value.toBoolean());
            if (value.isString()) return (GenericValueContainer<T>) new GenericValueContainer<>(value.toString());
        }

        throw new IllegalArgumentException("Javascript value could not be converted to a Java value");
    }
}
