package net.labymedia.ultralight.utils;

import net.labymedia.ultralight.Databind;
import net.labymedia.ultralight.DatabindJavascriptClass;
import net.labymedia.ultralight.javascript.*;

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
         || javaClass == Double.class) return context.makeNumber((double) object);

        if (javaClass == String.class) return context.makeString((String) object);
        if (javaClass == JavascriptValue.class) return (JavascriptValue) object;
        if (javaClass == JavascriptObject.class) return (JavascriptObject) object;

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

    public Object fromJavascript(JavascriptValue value) {
        if (value.isNull()) return null;
        if (value.isUndefined()) return JavascriptType.UNDEFINED;

        if (value.isBoolean()) return value.toBoolean();
        if (value.isNumber()) return value.toNumber();

        if (value.isString()) return value.toString();

        if (value.isObject()) {
            DatabindJavascriptClass.Data privateData = (DatabindJavascriptClass.Data) value.toObject().getPrivate();

            if (privateData.instance() == null) {
                return privateData.javaClass();
            } else {
                return privateData.instance();
            }
        }

        throw new IllegalArgumentException("Javascript value could not be converted to a Java object");
    }
}
