package net.janrupf.ultralight.javascript;

/**
 * Exceptions which can be thrown during the evaluation of javascript.
 */
public class JavascriptException extends Exception {
    /**
     * Constructs a new {@link JavascriptException} with just a message.
     *
     * @param message The message to display to the user
     */
    public JavascriptException(String message) {
        super(message);
    }

    /**
     * Constructs a new {@link JavascriptException} with a message and a cause.
     *
     * @param message The message to display to the user
     * @param cause   The exception causing this exception
     */
    public JavascriptException(String message, Throwable cause) {
        super(message, cause);
    }
}
