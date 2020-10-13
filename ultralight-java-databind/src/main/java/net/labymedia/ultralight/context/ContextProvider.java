package net.labymedia.ultralight.context;

import net.labymedia.ultralight.javascript.JavascriptContext;

import java.util.function.Consumer;

/**
 * Interface to be implemented by the user.
 */
public interface ContextProvider {
    /**
     * Schedules a function requiring a Javascript context for invocation. This method might be called
     * from any thread at any point. The implementor is responsible for moving the callback back to another thread
     * if required.
     *
     * @param function The function to invoke
     */
    void invoke(Consumer<JavascriptContext> function);
}
