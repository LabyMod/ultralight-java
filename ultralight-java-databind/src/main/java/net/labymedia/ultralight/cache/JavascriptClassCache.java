/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
