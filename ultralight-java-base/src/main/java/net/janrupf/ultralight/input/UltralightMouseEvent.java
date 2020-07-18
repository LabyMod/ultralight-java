package net.janrupf.ultralight.input;

import java.lang.annotation.Native;

/**
 * A generic mouse event.
 *
 * @see net.janrupf.ultralight.UltralightView#fireMouseEvent(UltralightMouseEvent)
 */
public class UltralightMouseEvent {
    /**
     * The type of this MouseEvent
     */
    @Native
    private UltralightMouseEventType type;

    /**
     * The current x-position of the mouse, relative to the View
     */
    @Native
    private int x;

    /**
     * The current y-position of the mouse, relative to the View
     */
    @Native
    private int y;

    /**
     * The mouse button that was pressed/released, if any, else {@code null}
     */
    @Native
    private UltralightMouseEventButton button;

    /**
     * Sets the type field of this instance.
     *
     * @param type The new value of the field
     * @return this
     * @see #type
     */
    public UltralightMouseEvent type(UltralightMouseEventType type) {
        this.type = type;
        return this;
    }

    /**
     * Sets the x field of this instance.
     *
     * @param x The new value of the field
     * @return this
     * @see #x
     */
    public UltralightMouseEvent x(int x) {
        this.x = x;
        return this;
    }

    /**
     * Sets the y field of this instance.
     *
     * @param y The new value of the field
     * @return this
     * @see #y
     */
    public UltralightMouseEvent y(int y) {
        this.y = y;
        return this;
    }

    /**
     * Sets the button field of this instance.
     *
     * @param button The new value of the field
     * @return this
     * @see #button
     */
    public UltralightMouseEvent button(UltralightMouseEventButton button) {
        this.button = button;
        return this;
    }
}
