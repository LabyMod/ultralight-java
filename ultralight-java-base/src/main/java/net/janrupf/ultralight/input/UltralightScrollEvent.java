package net.janrupf.ultralight.input;

import java.lang.annotation.Native;

import net.janrupf.ultralight.annotation.NativeType;

/**
 * A generic scroll event.
 */
@NativeType("ultralight::ScrollEvent")
public class UltralightScrollEvent {
    /**
     * Scroll granularity type
     */
    @Native
    private UltralightScrollEventType type;

    /**
     * Horizontal scroll amount
     */
    @Native
    private int deltaX;

    /**
     * Vertical scroll amount
     */
    @Native
    private int deltaY;

    /**
     * Sets the type field of this instance.
     *
     * @param type The new value of the field
     * @return this
     * @see #type
     */
    public UltralightScrollEvent type(UltralightScrollEventType type) {
        this.type = type;
        return this;
    }

    /**
     * Sets the deltaX field of this instance.
     *
     * @param deltaX The new value of the field
     * @return this
     * @see #deltaX
     */
    public UltralightScrollEvent deltaX(int deltaX) {
        this.deltaX = deltaX;
        return this;
    }

    /**
     * Sets the deltaY field of this instance.
     *
     * @param deltaY The new value of the field
     * @return this
     * @see #deltaY
     */
    public UltralightScrollEvent deltaY(int deltaY) {
        this.deltaY = deltaY;
        return this;
    }
}
