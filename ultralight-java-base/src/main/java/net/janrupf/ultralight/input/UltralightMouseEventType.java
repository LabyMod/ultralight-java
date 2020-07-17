package net.janrupf.ultralight.input;

import net.janrupf.ultralight.annotation.NativeType;

import java.lang.annotation.Native;

/**
 * The various MouseEvent types.
 */
@NativeType("ultralight::MouseEvent::Type")
public enum UltralightMouseEventType {
    /**
     * Mouse moved type
     */
    @Native
    MOVED,

    /**
     * Mouse button pressed type
     */
    @Native
    DOWN,

    /**
     * Mouse button released type
     */
    @Native
    UP
}
