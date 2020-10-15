package net.labymedia.ultralight.context;

import net.labymedia.ultralight.javascript.JavascriptContextLock;

/**
 * Interface to be implemented by the user.
 */
public interface ContextProvider {
    /**
     * Retrieves the bound Javascript context with a lock. The lock will be unlocked by the consumer automatically
     * after it finished using it.
     *
     * @return The bound Javascript context lock, or {@code null}, if the context is not available anymore
     */
    JavascriptContextLock getContext();
}
