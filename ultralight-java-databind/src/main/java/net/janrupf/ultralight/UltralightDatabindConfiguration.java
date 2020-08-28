package net.janrupf.ultralight;

public final class UltralightDatabindConfiguration {
    private boolean automaticPrototype;

    private UltralightDatabindConfiguration() { }

    public boolean automaticPrototype() {
        return automaticPrototype;
    }

    /**
     * Create a new <code>UltralightDatabindConfiguration</code> builder.
     *
     * @return a new builder.
     */
    public static Builder builder() {
        return new Builder();
    }

    /**
     * {@link UltralightDatabindConfiguration} builder.
     */
    public static class Builder {
        private final UltralightDatabindConfiguration configuration = new UltralightDatabindConfiguration();

        private Builder() { }

        /**
         * Specify whether missing prototypes should automatically be generated.
         *
         * @param automaticPrototype automatically generate prototypes
         * @return a {@link UltralightDatabindConfiguration} builder.
         */
        public Builder automaticPrototype(final boolean automaticPrototype) {
            configuration.automaticPrototype = automaticPrototype;
            return this;
        }

        /**
         * Build a {@link UltralightDatabindConfiguration}.
         *
         * @return a {@link UltralightDatabindConfiguration} instance based on specified values.
         */
        public UltralightDatabindConfiguration build() {
            return configuration;
        }
    }
}
