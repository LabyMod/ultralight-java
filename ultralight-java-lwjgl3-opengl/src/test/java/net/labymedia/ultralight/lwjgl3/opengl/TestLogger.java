package net.labymedia.ultralight.lwjgl3.opengl;

import net.labymedia.ultralight.plugin.logging.UltralightLogLevel;
import net.labymedia.ultralight.plugin.logging.UltralightLogger;

/**
 * Test implementation of a logger
 */
public class TestLogger implements UltralightLogger {
    @Override
    public void logMessage(UltralightLogLevel level, String message) {
        switch (level) {
            case ERROR:
                System.err.println("[Ultralight/ERR] " + message);
                break;

            case WARNING:
                System.err.println("[Ultralight/WARN] " + message);
                break;

            case INFO:
                System.out.println("[Ultralight/INFO] " + message);
                break;
        }
    }
}
