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

import com.labymedia.ultralight.annotation.NativeCall;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.ffi.ObjectWithHandle;
import com.labymedia.ultralight.ffi.gc.DeletableObject;

/**
 * Transformation Matrix helper.
 */
@NativeType("ultralight::Matrix")
public class UltralightMatrix implements ObjectWithHandle {
    private final DeletableObject<Long> handle;

    /**
     * Constructs a new {@link UltralightMatrix} and begins tracking the underlying handle.
     *
     * @param handle A pointer to a native ultralight::Matrix, the java object will take
     *               ownership of the native pointer
     */
    @NativeCall
    private UltralightMatrix(long handle) {
        this.handle = new DeletableObject<>(handle, UltralightMatrix::delete);
    }

    /**
     * Constructs a new, empty matrix.
     */
    public UltralightMatrix() {
        this.handle = new DeletableObject<>(construct(), UltralightMatrix::delete);
    }

    /**
     * Sets the values of the matrix.
     *
     * @param m11 row 1, column 1
     * @param m12 row 1, column 2
     * @param m13 row 1, column 3
     * @param m14 row 1, column 4
     * @param m21 row 2, column 1
     * @param m22 row 2, column 2
     * @param m23 row 2, column 3
     * @param m24 row 2, column 4
     * @param m31 row 3, column 1
     * @param m32 row 3, column 2
     * @param m33 row 3, column 3
     * @param m34 row 3, column 4
     * @param m41 row 4, column 1
     * @param m42 row 4, column 2
     * @param m43 row 4, column 3
     * @param m44 row 4, column 4
     */
    public native void set(
            @NativeType("double") double m11, @NativeType("double") double m12, @NativeType(
            "double") double m13, @NativeType("double") double m14,
            @NativeType("double") double m21, @NativeType("double") double m22, @NativeType(
            "double") double m23, @NativeType("double") double m24,
            @NativeType("double") double m31, @NativeType("double") double m32, @NativeType(
            "double") double m33, @NativeType("double") double m34,
            @NativeType("double") double m41, @NativeType("double") double m42, @NativeType(
            "double") double m43, @NativeType("double") double m44
    );

    /**
     * Sets the values of this matrix by coping from a 4x4 matrix.
     *
     * @param ultralightMatrix4x4 The matrix to copy from
     */
    public native void set(UltralightMatrix4x4 ultralightMatrix4x4);

    /**
     * Set to an orthographic projection matrix suitable for use with our
     * vertex shaders. Optionally flip the y-coordinate space (eg, for OpenGL).
     *
     * @param screenWidth  The width of the projection
     * @param screenHeight The height of the projection
     * @param flipY        Whether the y coordinate space should be flipped
     */
    public native void setOrthographicProjection(@NativeType("double") double screenWidth, @NativeType(
            "double") double screenHeight, @NativeType("bool") boolean flipY);

    /**
     * Transforms this matrix with another matrix.
     *
     * @param transformMat The matrix to transform with
     */
    public native void transform(UltralightMatrix transformMat);

    /**
     * Retrieves this matrix as a 4x4 matrix.
     *
     * @return This matrix as a 4x4 matrix
     */
    public native UltralightMatrix4x4 getMatrix4x4();

    /**
     * Constructs a native instance of the matrix.
     *
     * @return A pointer to the crated instance.
     */
    private static native long construct();

    /**
     * Executes the deletion of the native `ultralight::Matrix` instance.
     * This method is static to not keep a reference to the java object, which
     * else would prevent deletion.
     *
     * @param handle A pointer to the instance to delete
     */
    private static native void delete(long handle);

    @Override
    @NativeType("ultralight::Matrix *")
    public long getHandle() {
        return handle.get();
    }

    // TODO: Other methods
}
