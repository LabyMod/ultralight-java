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

package com.labymedia.ultralight.javascript;

import com.labymedia.ultralight.annotation.NativeType;

import java.lang.annotation.Native;

/**
 * A constant identifying the Typed Array type of a {@link JavascriptObject}.
 */
@NativeType("JSTypedArray")
public enum JavascriptTypedArrayType {
    /**
     * Array of 8bit integers. Can be represented in Java by a {@code byte[]}.
     */
    @Native
    INT8,

    /**
     * Array of 16bit integers. Can be represented in Java by a {@code short[]}.
     */
    @Native
    INT16,

    /**
     * Array of 32bit integers. Can be represented in Java by a {@code int[]}.
     */
    @Native
    INT32,

    /**
     * Array of unsigned 8bit integers. Can't be directly represented by java, as the content is unsigned. The closest
     * representation is a {@code short[]} with only positive values.
     */
    @Native
    UINT8,

    /**
     * Array of unsigned 8bit integers clamped to 255. Can't be directly represented by java, as the content is
     * unsigned. The closest representation is a {@code short[]} with only positive values.
     */
    @Native
    UINT8_CLAMPED,

    /**
     * Array of unsigned 16bit integers. Can't be directly represented by java, as the content is unsigned. The closest
     * representation is a {@code int[]} with only positive values.
     */
    @Native
    UINT16,

    /**
     * Array of unsigned 8bit integers. Can't be directly represented by java, as the content is unsigned. The closest
     * representation is a {@code long[]} with only positive values.
     */
    @Native
    UINT32,

    /**
     * Array of 32bit floats. Can by represented in Java by a {@code float[]}.
     */
    @Native
    FLOAT32,

    /**
     * Array of 64bit floats. Can be represented in Java by a {@code double[]}.
     */
    @Native
    FLOAT64,

    /**
     * Region of binary data. Can be represented in Java by a {@link java.nio.ByteBuffer}.
     */
    @Native
    BUFFER,

    /**
     * Not a typed array.
     */
    @Native
    NONE
}
