/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 - 2021 LabyMedia and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

package com.labymedia.ultralight.databind;

import com.labymedia.ultralight.databind.cache.JavascriptClassCache;
import com.labymedia.ultralight.databind.context.ContextProviderFactory;
import com.labymedia.ultralight.databind.utils.JavascriptConversionUtils;
import com.labymedia.ultralight.javascript.JavascriptClass;

/**
 * Representation of Databind instances.
 */
public final class Databind {
    private final DatabindConfiguration configuration;
    private final JavascriptClassCache classCache;
    private final ContextProviderFactory contextProviderFactory;

    private final JavascriptConversionUtils conversionUtils;

    /**
     * Constructs a new {@link Databind} instance using the given configuration.
     *
     * @param configuration The configuration used by this {@link Databind} instance
     */
    public Databind(DatabindConfiguration configuration) {
        this.configuration = configuration;
        this.classCache = configuration.classCache();
        this.contextProviderFactory = configuration.contextProviderFactory();
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
                    DatabindJavascriptClass.create(configuration, conversionUtils, javaClass, classCache)
                            .bake());
        }

        return javascriptClass;
    }

    /**
     * Retrieves the conversion utilities which can  be used to convert single objects.
     *
     * @return The conversion utilities
     */
    public JavascriptConversionUtils getConversionUtils() {
        return conversionUtils;
    }

    /**
     * Retrieves the context provider factory which can be used to bind object contexts.
     *
     * @return The context provider factory
     */
    public ContextProviderFactory getContextProviderFactory() {
        return contextProviderFactory;
    }

    /**
     * Determines whether functional conversion is supported (depends on whether a context provider factory
     * is available).
     *
     * @return {@code true} if Javascript functions can be converted to Java functional interfaces, {@code false}
     * otherwise
     */
    public boolean supportsFunctionalConversion() {
        return contextProviderFactory != null;
    }
}
