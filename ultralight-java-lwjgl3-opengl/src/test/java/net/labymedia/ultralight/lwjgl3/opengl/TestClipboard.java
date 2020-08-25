package net.labymedia.ultralight.lwjgl3.opengl;

import net.labymedia.ultralight.plugin.clipboard.UltralightClipboard;

import static org.lwjgl.glfw.GLFW.*;

/**
 * Test clipboard using GLFW as backend.
 */
public class TestClipboard implements UltralightClipboard {
    @Override
    public void clear() {
        glfwSetClipboardString(0, "");
    }

    @Override
    public String readPlainText() {
        return glfwGetClipboardString(0);
    }

    @Override
    public void writePlainText(String text) {
        glfwSetClipboardString(0, text);
    }
}
