package net.labymedia.ultralight;

import net.labymedia.ultralight.cache.NaiveJavascriptClassCache;
import net.labymedia.ultralight.cache.JavascriptClassCache;
import net.labymedia.ultralight.call.HeuristicMethodChooser;
import net.labymedia.ultralight.call.MethodChooser;

/**
 * Databind configuration.
 */
public final class DatabindConfiguration {
    private final JavascriptClassCache classCache;
    private final MethodChooser methodChooser;
    private final boolean automaticPrototype;

    /**
     * Constructs a new {@link DatabindConfiguration}.
     * Use the {@link Builder} for creating instances outside of this class
     *
     * @param classCache The class cache used by this configuration
     * @param methodChooser The method chooser used by this configuration
     * @param automaticPrototype If {@code true}, automatic prototyping is enabled
     */
    private DatabindConfiguration(
            JavascriptClassCache classCache, MethodChooser methodChooser, boolean automaticPrototype) {
        this.classCache = classCache;
        this.methodChooser = methodChooser;
        this.automaticPrototype = automaticPrototype;
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
         * Builds a {@link DatabindConfiguration}.
         *
         * @return The built {@link DatabindConfiguration}
         */
        public DatabindConfiguration build() {
            return new DatabindConfiguration(classCache, methodChooser, automaticPrototype);
        }
    }
}
