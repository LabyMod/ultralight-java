package net.labymedia.ultralight.context;

import net.labymedia.ultralight.javascript.JavascriptValue;

/**
 * Interface to be implemented by the user for binding values in callbacks to contexts.
 * <p>
 * This is required for translating Javascript methods to functional interfaces and invoking them, as the context
 * gets lost in the process of binding the interface. The library will then invoke the context provider factory
 * to re-acquire the context.
 * <p>
 * @see net.labymedia.ultralight.DatabindConfiguration.Builder#contextProviderFactory(ContextProviderFactory)
 */
public interface ContextProviderFactory {
    /**
     * Binds a provider for the specified Javascript value.
     *
     * @param value The value to bind a provider for
     * @return The bound provider
     */
    ContextProvider bindProvider(JavascriptValue value);
}
