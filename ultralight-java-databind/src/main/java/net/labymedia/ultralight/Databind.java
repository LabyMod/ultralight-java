package net.labymedia.ultralight;

import net.labymedia.ultralight.cache.JavascriptClassCache;
import net.labymedia.ultralight.javascript.JavascriptClass;
import net.labymedia.ultralight.utils.JavascriptConversionUtils;

public final class Databind {
    private final DatabindConfiguration configuration;
    private final JavascriptClassCache classCache;

    private final JavascriptConversionUtils conversionUtils;

    public Databind(DatabindConfiguration configuration) {
        this.configuration = configuration;
        this.classCache = configuration.classCache();
        this.conversionUtils = new JavascriptConversionUtils(this);
    }

    public JavascriptClass toJavascript(Class<?> javaClass) {
        return toJavascript(javaClass, true);
    }

    public JavascriptClass toJavascript(Class<?> javaClass, boolean ignoreAutomaticPrototypeSetting) {
        return toJavascript(javaClass.getName(), javaClass, ignoreAutomaticPrototypeSetting);
    }

    public JavascriptClass toJavascript(String name, Class<?> javaClass) {
        return toJavascript(name, javaClass, true);
    }

    public JavascriptClass toJavascript(String name, Class<?> javaClass, boolean ignoreAutomaticPrototypeSetting) {
        JavascriptClass javascriptClass = classCache.get(name);

        if (javascriptClass == null) {
            if (!ignoreAutomaticPrototypeSetting && configuration.automaticPrototype()) throw new IllegalStateException("Cannot automatically import a prototype due to databind configuration");
            javascriptClass = classCache.put(name, DatabindJavascriptClass.create(configuration, conversionUtils, name, javaClass).bake());
        }

        return javascriptClass;
    }

    public DatabindConfiguration configuration() {
        return configuration;
    }
}
