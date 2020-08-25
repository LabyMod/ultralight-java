package net.labymedia.ultralight;

/**
 * Exception that can occur while trying to load the native library.
 */
public class UltralightLoadException extends Exception {
    private static final long serialVersionUID = 5174097344028286712L;

    /**
     * Constructs a new {@link UltralightLoadException} with just a message.
     *
     * @param message The message to display to the user
     */
    public UltralightLoadException(String message) {
        super(message);
    }

    /**
     * Constructs a new {@link UltralightLoadException} with a message and a cause.
     *
     * @param message The message to display to the user
     * @param cause The exception causing this exception
     */
    public UltralightLoadException(String message, Throwable cause) {
        super(message, cause);
    }
}
