package net.labymedia.ultralight.cache;

import net.labymedia.ultralight.javascript.JavascriptClass;

/**
 * Map like cache structure for caching Javascript classes.
 */
public interface JavascriptClassCache {
    /**
     * Retrieves a class from the cache.
     *
     * @param name The name of the class to retrieve
     * @return The cached class, or {@code null}, if the class is not cached
     */
    JavascriptClass get(String name);

    /**
     * Puts a class into the cache.
     *
     * @param name The name of the class to cache
     * @param javascriptClass The class instance to cache
     * @return javascriptClass
     */
    JavascriptClass put(String name, JavascriptClass javascriptClass);

    /**
     * Removes a class from the cache.
     *
     * @param name The name of the class to remove
     * @return The previously cached class, or {@code null}, if the class was not cached
     */
    JavascriptClass delete(String name);

    /**
     * Determines whether the cache contains a class.
     *
     * @param name The name of the class to test for
     * @return {@code true} if the class is cached, {@code false} otherwise
     */
    boolean contains(String name);
}
