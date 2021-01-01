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
import com.labymedia.ultralight.annotation.Unsigned;

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
