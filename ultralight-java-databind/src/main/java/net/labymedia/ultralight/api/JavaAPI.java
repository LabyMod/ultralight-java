package net.labymedia.ultralight.api;

import net.labymedia.ultralight.Databind;
import net.labymedia.ultralight.DatabindJavascriptClass;
import net.labymedia.ultralight.javascript.JavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptObject;

public final class JavaAPI {
    private final Databind databind;

    public JavaAPI(Databind databind) {
        this.databind = databind;
    }

    @InjectJavascriptContext
    public JavascriptObject importClass(JavascriptContext context, String name, String className) throws ClassNotFoundException {
        Class<?> javaClass = Class.forName(className);

        return context.makeObject(databind.toJavascript(name, javaClass), DatabindJavascriptClass.Data.builder()
            .javaClass(javaClass)
            .build());
    }

    @InjectJavascriptContext
    public JavascriptObject importClass(JavascriptContext context, String className) throws ClassNotFoundException {
        Class<?> javaClass = Class.forName(className);

        return context.makeObject(databind.toJavascript(javaClass), DatabindJavascriptClass.Data.builder()
            .javaClass(javaClass)
            .build());
    }
}
