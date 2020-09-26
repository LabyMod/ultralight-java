package net.labymedia.ultralight.cache;

import net.labymedia.ultralight.javascript.JavascriptClass;

import java.util.HashMap;
import java.util.Map;

public final class NaiveJavascriptClassCache implements JavascriptClassCache {
    private final Map<String, JavascriptClass> cache = new HashMap<>();

    @Override
    public JavascriptClass get(String name) {
        return cache.get(name);
    }

    @Override
    public JavascriptClass put(String name, JavascriptClass javascriptClass) {
        cache.put(name, javascriptClass);
        return javascriptClass;
    }

    @Override
    public JavascriptClass delete(String name) {
        return cache.remove(name);
    }

    @Override
    public boolean contains(String name) {
        return cache.containsKey(name);
    }
}
