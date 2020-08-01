package net.janrupf.ultralight.javascript.interop;

/**
 * Exception which might by thrown by user code when interop'ing Java and Javascript.
 */
public class JavascriptInteropException extends Exception {
    /**
     * Constructs a new {@link JavascriptInteropException} with the given message.
     *
     * @param message The message to pass on to Javascript
     */
    public JavascriptInteropException(String message) {
        super(message);
    }

    /**
     * Constructs a new {@link JavascriptInteropException} with the given message caused by another exception.
     *
     * @param message The message to pass on to Javascript
     * @param cause The exception causing this exception
     */
    public JavascriptInteropException(String message, Throwable cause) {
        super(message, cause);
    }
}
