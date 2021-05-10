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

import com.labymedia.ultralight.annotation.NativeType;

import java.lang.annotation.Native;

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
