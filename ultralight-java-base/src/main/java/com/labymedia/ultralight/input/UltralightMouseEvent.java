/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 - 2021 LabyMedia and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

package com.labymedia.ultralight.input;

import com.labymedia.ultralight.UltralightView;

import java.lang.annotation.Native;

/**
 * A generic mouse event.
 *
 * @see UltralightView#fireMouseEvent(UltralightMouseEvent)
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
