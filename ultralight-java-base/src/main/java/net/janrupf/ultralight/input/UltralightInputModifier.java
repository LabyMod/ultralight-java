package net.janrupf.ultralight.input;

import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.annotation.Unsigned;

/**
 * An enumeration of the different keyboard modifiers.
 */
public final class UltralightInputModifier {
    /**
     * Whether or not an ALT key is down
     */
    @NativeType("unsigned")
    @Unsigned
    public static final int ALT_KEY = 1;

    /**
     * Whether or not a Control key is down
     */
    @NativeType("unsigned")
    @Unsigned
    public static final int CTRL_KEY = 1 << 1;

    /**
     * Whether or not a meta key (Command-key on Mac, Windows-key on Win) is down
     */
    @NativeType("unsigned")
    @Unsigned
    public static final int META_KEY = 1 << 2;

    /**
     * Whether or not a Shift key is down
     */
    @NativeType("unsigned")
    @Unsigned
    public static final int SHIFT_KEY = 1 << 3;
}
