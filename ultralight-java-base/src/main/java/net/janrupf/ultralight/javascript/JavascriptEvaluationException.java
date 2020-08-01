package net.janrupf.ultralight.javascript;

import net.janrupf.ultralight.annotation.NativeCall;

/**
 * Exceptions which can be thrown during the evaluation of javascript.
 */
public class JavascriptEvaluationException extends Exception {
    private static final long serialVersionUID = 250417540756557840L;

    /**
     * Constructs a new {@link JavascriptEvaluationException} with just a message.
     *
     * @param message The message to display to the user
     */
    @NativeCall
    public JavascriptEvaluationException(String message) {
        super(message);
    }

    /**
     * Constructs a new {@link JavascriptEvaluationException} with a message and a cause.
     *
     * @param message The message to display to the user
     * @param cause   The exception causing this exception
     */
    public JavascriptEvaluationException(String message, Throwable cause) {
        super(message, cause);
    }
}
