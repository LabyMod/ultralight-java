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

package com.labymedia.ultralight.math;

import com.labymedia.ultralight.annotation.NativeType;

import java.util.Arrays;

/**
 * 4D Vector Helper.
 */
@NativeType("ultralight::Vec4")
public class Vec4 {
    private final float[] value = new float[4];

    /**
     * Constructs a new, empty 4D vector.
     */
    public Vec4() {
    }

    /**
     * Constructs a new 4D vector with the components set.
     *
     * @param x The first vector component
     * @param y The second vector component
     * @param z The third vector component
     * @param w The forth vector component
     */
    public Vec4(float x, float y, float z, float w) {
        this.set(x, y, z, w);
    }

    /**
     * Constructs a new 4D vector by setting all its components to the same value.
     *
     * @param value The value to set all vector components to
     */
    public Vec4(float value) {
        set(value, value, value, value);
    }

    /**
     * Constructs a new 4D vector by copying the values from an array.
     *
     * @param value The array to copy the values from, must be exactly 4 floats
     */
    public Vec4(float[] value) {
        if (value.length != 4) {
            throw new IllegalArgumentException("Expected value to be exactly 4 floats");
        }

        System.arraycopy(value, 0, this.value, 0, 4);
    }

    /**
     * Sets the components of the vector.
     *
     * @param x The first vector component
     * @param y The second vector component
     * @param z The third vector component
     * @param w The forth vector component
     */
    public void set(float x, float y, float z, float w) {
        this.value[0] = x;
        this.value[1] = y;
        this.value[2] = z;
        this.value[3] = w;
    }

    /**
     * Retrieves the value of the vector as a float array.
     * Can be used to mutate the vector.
     *
     * @return The value of this vector
     * @see #getValueCopy()
     */
    public float[] getValue() {
        return value;
    }

    /**
     * Copies the value of the vector as a float array.
     *
     * @return The value of this vector
     * @see #getValue()
     */
    public float[] getValueCopy() {
        float[] copy = new float[4];
        System.arraycopy(value, 0, copy, 0, 4);
        return copy;
    }

    /**
     * Retrieves the first component of the vector.
     *
     * @return The first component of the vector
     */
    public float getX() {
        return value[0];
    }

    /**
     * Retrieves the second component of the vector.
     *
     * @return The second component of the vector
     */
    public float getY() {
        return value[1];
    }

    /**
     * Retrieves the third component of the vector.
     *
     * @return The third component of the vector
     */
    public float getZ() {
        return value[2];
    }

    /**
     * Retrieves the forth component of the vector.
     *
     * @return The forth component of the vector
     */
    public float getW() {
        return value[3];
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Vec4 vec4 = (Vec4) o;
        return Arrays.equals(value, vec4.value);
    }

    @Override
    public int hashCode() {
        return Arrays.hashCode(value);
    }
}
