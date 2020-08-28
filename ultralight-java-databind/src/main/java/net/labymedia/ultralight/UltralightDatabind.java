package net.labymedia.ultralight;

import net.labymedia.ultralight.containers.CustomJavascriptClassDefinition;
import net.labymedia.ultralight.javascript.*;

public final class UltralightDatabind {
    private final UltralightDatabindConfiguration configuration;

    public UltralightDatabind(final UltralightDatabindConfiguration configuration) {
        this.configuration = configuration;
    }

    public JavascriptClass toJavascript(final Object javaObject) throws IllegalAccessException {
        return CustomJavascriptClassDefinition.from(javaObject).staticDefinition().bake();
    }

    /*
     * context:
     *
     * let o = context.makeObject(<JavascriptClass>, <Object>: private data);
     * o.getPrivate() -> private data
     * #####
     * java.io.File -> JavascriptObject
     *
     * let javaFile = new java.io.File("blub");
     * let javascriptFile = context.makeContext(javascriptJavaIoFileClass, javaFile);
     *
     * -----
     * JS[javascriptFile.getParent()] -> Java[onGetProperty("getParent")]
     *                                => JavascriptObject("java.io.File#getParent")
     *                                -> Java[onCallAsFunction(context, thisObject)]
     *                                => thisObject.getPrivate() -> java.io.File
     *                                -> Java[file.getParent()]: java.io.File
     *                                => toJavascript(returnValue)
     * -> java.io.File as Javascript
     */

    public JavascriptClass toJavascript(final Object javaObject, final Class<?> superClass) {
        return null;
    }
}
