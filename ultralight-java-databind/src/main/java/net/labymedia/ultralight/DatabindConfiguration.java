package net.labymedia.ultralight;

import net.labymedia.ultralight.cache.NaiveJavascriptClassCache;
import net.labymedia.ultralight.cache.JavascriptClassCache;
import net.labymedia.ultralight.utils.HeuristicMethodChooser;
import net.labymedia.ultralight.utils.MethodChooser;

/**
 * Databind configuration.
 */
public final class DatabindConfiguration {
    private JavascriptClassCache classCache = new NaiveJavascriptClassCache();
    private MethodChooser methodChooser = new HeuristicMethodChooser();
    private boolean automaticPrototype = true;

    private DatabindConfiguration() { }

    public JavascriptClassCache classCache() {
        return classCache;
    }

    public MethodChooser methodChooser() {
        return methodChooser;
    }

    public boolean automaticPrototype() {
        return automaticPrototype;
    }

    /**
     * Create a new {@link DatabindConfiguration} builder.
     *
     * @return a new builder.
     */
    public static Builder builder() {
        return new Builder();
    }

    /**
     * {@link DatabindConfiguration} builder.
     */
    public static class Builder {
        private DatabindConfiguration configuration = new DatabindConfiguration();

        private Builder() { }

        public Builder classCache(JavascriptClassCache classCache) {
            configuration.classCache = classCache;
            return this;
        }

        public Builder methodChooser(MethodChooser methodChooser) {
            configuration.methodChooser = methodChooser;
            return this;
        }

        public Builder automaticPrototype(boolean automaticPrototype) {
            configuration.automaticPrototype = automaticPrototype;
            return this;
        }

        /**
         * Build a {@link DatabindConfiguration}.
         *
         * @return a {@link DatabindConfiguration} instance based on specified values.
         */
        public DatabindConfiguration build() {
            return configuration;
        }
    }
}
