package net.labymedia.ultralight.input;

import net.labymedia.ultralight.annotation.NativeType;

import java.lang.annotation.Native;

/**
 * The various KeyEvent types.
 */
@NativeType("ultralight::KeyEvent::Type")
public enum UltralightKeyEventType {
    /**
     * Key-Down type
     */
    @Native
    DOWN,

    /**
     * Key-Up type
     */
    @Native
    UP,

    /**
     * Raw Key-Down type
     */
    @Native
    RAW_DOWN,

    /**
     * Character input type (this event generates text in input fields)
     */
    @Native
    CHAR
}
