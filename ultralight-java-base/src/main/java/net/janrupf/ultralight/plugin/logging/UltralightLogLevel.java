package net.janrupf.ultralight.plugin.logging;

/**
 * Log levels, used with {@link UltralightLogger#logMessage(UltralightLogLevel, String)}
 */
public enum UltralightLogLevel {
    /**
     * An error occurred and a message has been generated.
     */
    ERROR,

    /**
     * Some warning has been triggered and a message has been generated.
     */
    WARNING,

    /**
     * An event occurred and a message has been generated
     */
    INFO
}
