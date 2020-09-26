package net.labymedia.ultralight.cache;

import net.labymedia.ultralight.javascript.JavascriptClass;

public interface JavascriptClassCache {
    JavascriptClass get(String name);
    JavascriptClass put(String name, JavascriptClass javascriptClass);
    JavascriptClass delete(String name);
    boolean contains(String name);
}
