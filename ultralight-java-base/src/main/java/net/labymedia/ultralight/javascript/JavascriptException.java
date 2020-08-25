package net.labymedia.ultralight.javascript;

import net.labymedia.ultralight.annotation.NativeCall;

/**
 * Exception bridged from Javascript. Handling these is difficult and should be done as soon as possible,
 * as the contained exception value gets lost as soon as the context unlocks.
 * <p>
 * As Javascript exceptions, this exception is unchecked since it is not known when it can occur.
 */
public class JavascriptException extends RuntimeException {
    private final JavascriptValue value;

    /**
     * Constructs a new {@link JavascriptException}
     * @param message The message describing when the exception occurred
     * @param value The value that has been thrown by Javascript
     */
    @NativeCall
    public JavascriptException(String message, JavascriptValue value) {
        super(message);
        this.value = value;
    }

    /**
     * Retrieves the Javascript native value of this exception.
     *
     * @return The Javascript native value of this exception
     */
    public JavascriptValue getValue() {
        return value;
    }
}
