package net.janrupf.ultralight.lwjgl3.opengl;

import java.io.IOException;

/**
 * Exception which may occur during loading of shaders.
 */
public class ShaderLoadException extends IOException {
    private static final long serialVersionUID = -1695228181707402338L;

    /**
     * Constructs a new {@link ShaderLoadException} with just a message.
     *
     * @param message The message to display to the user
     */
    public ShaderLoadException(String message) {
        super(message);
    }

    /**
     * Constructs a new {@link ShaderLoadException} with a message and a cause.
     *
     * @param message The message to display to the user
     * @param cause The exception causing this exception
     */
    public ShaderLoadException(String message, Throwable cause) {
        super(message, cause);
    }
}
