package net.janrupf.ultralight.plugin.logging;

import net.janrupf.ultralight.annotation.NativeType;

/**
 * Logger interface.
 * <p>
 * This can be used to log debug messages to the console or to a log file.
 * <p>
 * This is intended to be implemented by users and defined before creating the
 * Renderer.
 */
@NativeType("ultralight::Logger")
public interface UltralightLogger {
    /**
     * Called when the library wants to print a message to the log.
     *
     * @param level   The level of the message
     * @param message The message to log
     */
    void logMessage(UltralightLogLevel level, String message);
}
