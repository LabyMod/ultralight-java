package net.labymedia.ultralight.api;

import net.labymedia.ultralight.Databind;
import net.labymedia.ultralight.javascript.JavascriptClass;

// TODO: Rewrite this to meta JavaScript.
public final class JavaAPI {
    private final Databind databind;

    public JavaAPI(Databind databind) {
        this.databind = databind;
    }

    // Returns JavascriptClass. Cannot pass down the Java class and loses metadata in the JavascriptConversionUtils
    // class.
    public JavascriptClass importClass(String name, String className) throws ClassNotFoundException {
        return databind.toJavascript(name, Class.forName(className));
    }

    public JavascriptClass importClass(String className) throws ClassNotFoundException {
        return databind.toJavascript(Class.forName(className));
    }
}
