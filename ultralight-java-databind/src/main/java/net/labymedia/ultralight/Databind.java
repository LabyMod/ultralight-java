package net.labymedia.ultralight;

import net.labymedia.ultralight.cache.JavascriptClassCache;
import net.labymedia.ultralight.javascript.JavascriptClass;
import net.labymedia.ultralight.utils.JavascriptConversionUtils;

/**
 * Representation of Databind instances.
 */
public final class Databind {
    private final DatabindConfiguration configuration;
    private final JavascriptClassCache classCache;

    private final JavascriptConversionUtils conversionUtils;

    /**
     * Constructs a new {@link Databind} instance using the given configuration.
     *
     * @param configuration The configuration used by this {@link Databind} instance
     */
    public Databind(DatabindConfiguration configuration) {
        this.configuration = configuration;
        this.classCache = configuration.classCache();
        this.conversionUtils = new JavascriptConversionUtils(this);
    }

    /**
     * Converts a Java class to a Javascript object.
     *
     * @param javaClass The Java class to convert
     * @return The converted Javascript class
     */
    public JavascriptClass toJavascript(Class<?> javaClass) {
        return toJavascript(javaClass, true);
    }

    /**
     * Converts a Java class to a Javascript object with optional automatic prototyping.
     *
     * @param javaClass                       The Java class to convert
     * @param ignoreAutomaticPrototypeSetting If {@code true}, the class is always converted,
     *                                        even if the configuration disabled it
     * @return The converted Javascript class
     */
    public JavascriptClass toJavascript(Class<?> javaClass, boolean ignoreAutomaticPrototypeSetting) {
        JavascriptClass javascriptClass = classCache.get(javaClass.getName());

        if (javascriptClass == null) {
            if (!ignoreAutomaticPrototypeSetting && configuration.automaticPrototype()) {
                throw new IllegalStateException("Cannot automatically import a prototype due to databind configuration");
            }

            javascriptClass = classCache.put(
                    javaClass.getName(),
                    DatabindJavascriptClass.create(configuration, conversionUtils, javaClass)
                            .bake());
        }

        return javascriptClass;
    }
}
