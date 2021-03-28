package com.labymedia.ultralight.plugin.render;

/**
 * Command types, used by {@link UltralightCommand#getCommandType()}.
 */
public enum UltralightCommandType {
    /**
     * The command should clear the render buffer.
     */
    CLEAR_RENDER_BUFFER,

    /**
     * The command should draw some geometry into the render buffer.
     */
    DRAW_GEOMETRY
}
