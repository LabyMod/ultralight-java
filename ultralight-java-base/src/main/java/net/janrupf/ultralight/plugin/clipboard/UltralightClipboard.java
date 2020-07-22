package net.janrupf.ultralight.plugin.clipboard;

/**
 * Clipboard interface.
 * <p>
 * This is used for reading and writing data to the platform Clipboard.
 * <p>
 * This is intended to be implemented by users and defined before creating the Renderer. @see Platform::set_clipboard.
 */
public interface UltralightClipboard {
    /**
     * Clear the clipboard.
     */
    void clear();

    /**
     * Read plain text from the clipboard
     *
     * @return The content of the clipboard as plain text
     */
    String readPlainText();

    /**
     * Write plain text to the clipboard.
     *
     * @param text The plain text to write to the clipboard
     */
    void writePlainText(String text);
}
