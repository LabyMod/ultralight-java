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

package net.labymedia.ultralight;

import net.labymedia.ultralight.cache.JavascriptClassCache;
import net.labymedia.ultralight.cache.NaiveJavascriptClassCache;
import net.labymedia.ultralight.call.HeuristicMethodChooser;
import net.labymedia.ultralight.call.MethodChooser;
import net.labymedia.ultralight.context.ContextProviderFactory;

/**
 * Databind configuration.
 */
public final class DatabindConfiguration {
    private final JavascriptClassCache classCache;
    private final MethodChooser methodChooser;
    private final boolean automaticPrototype;
    private final ContextProviderFactory contextProviderFactory;

    /**
     * Constructs a new {@link DatabindConfiguration}.
     * Use the {@link Builder} for creating instances outside of this class
     *
     * @param classCache             The class cache used by this configuration
     * @param methodChooser          The method chooser used by this configuration
     * @param automaticPrototype     If {@code true}, automatic prototyping is enabled
     * @param contextProviderFactory The factory for binding context providers, or {@code null}, if this feature
     *                               is not required
     */
    private DatabindConfiguration(
            JavascriptClassCache classCache,
            MethodChooser methodChooser,
            boolean automaticPrototype,
            ContextProviderFactory contextProviderFactory
    ) {
        this.classCache = classCache;
        this.methodChooser = methodChooser;
        this.automaticPrototype = automaticPrototype;
        this.contextProviderFactory = contextProviderFactory;
    }

    /**
     * Retrieves the class cache instance of this configuration.
     *
     * @return The class cache instance
     */
    public JavascriptClassCache classCache() {
        return classCache;
    }

    /**
     * Retrieves the method chooser instance of this configuration.
     *
     * @return The method chooser instance
     */
    public MethodChooser methodChooser() {
        return methodChooser;
    }

    /**
     * Determines if this configuration has automatic prototyping enabled.
     *
     * @return {@code true} if automatic prototyping is enabled, {@code false} otherwise
     */
    public boolean automaticPrototype() {
        return automaticPrototype;
    }

    /**
     * Retrieves the context provider factory instance of this configuration.
     *
     * @return The context provider factory instance
     */
    public ContextProviderFactory contextProviderFactory() {
        return contextProviderFactory;
    }

    /**
     * Creates a new {@link DatabindConfiguration} builder.
     *
     * @return A new {@link DatabindConfiguration} builder
     */
    public static Builder builder() {
        return new Builder();
    }

    /**
     * Builder for {@link DatabindConfiguration} instances.
     */
    public static class Builder {
        private JavascriptClassCache classCache;
        private MethodChooser methodChooser;
        private boolean automaticPrototype;
        private ContextProviderFactory contextProviderFactory;

        /**
         * Constructs a new {@link Builder} with a default configuration.
         * Use {@link DatabindConfiguration#builder()} to create instances outside of this class.
         */
        private Builder() {
            this.classCache = new NaiveJavascriptClassCache();
            this.methodChooser = new HeuristicMethodChooser();
            this.automaticPrototype = true;
        }

        /**
         * Sets the class cache to be used by the configuration being built.
         *
         * @param classCache The class cache to use
         * @return this
         */
        public Builder classCache(JavascriptClassCache classCache) {
            this.classCache = classCache;
            return this;
        }

        /**
         * Sets the method chooser to be used by the configuration being built.
         *
         * @param methodChooser The method chooser to use
         * @return this
         */
        public Builder methodChooser(MethodChooser methodChooser) {
            this.methodChooser = methodChooser;
            return this;
        }

        /**
         * Configures whether the configuration being built will use automatic prototyping.
         *
         * @param automaticPrototype If {@code true}, this configuration will have automatic prototyping enabled and
         *                           automatically translate unknown classes.
         * @return this
         */
        public Builder automaticPrototype(boolean automaticPrototype) {
            this.automaticPrototype = automaticPrototype;
            return this;
        }

        /**
         * Sets the context provider factory to be used by the configuration being built.
         *
         * @param contextProviderFactory The context provider factory, or {@code null}, if this feature is not required
         * @return this
         */
        public Builder contextProviderFactory(ContextProviderFactory contextProviderFactory) {
            this.contextProviderFactory = contextProviderFactory;
            return this;
        }

        /**
         * Builds a {@link DatabindConfiguration}.
         *
         * @return The built {@link DatabindConfiguration}
         */
        public DatabindConfiguration build() {
            return new DatabindConfiguration(classCache, methodChooser, automaticPrototype, contextProviderFactory);
        }
    }
}
