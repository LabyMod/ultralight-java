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

@NativeType("ultralight::Matrix")
public class UltralightMatrix implements ObjectWithHandle {
    private final DeletableObject<Long> handle;

    @NativeCall
    private UltralightMatrix(long handle) {
        this.handle = new DeletableObject<>(handle, UltralightMatrix::delete);
    }

    public UltralightMatrix() {
        this.handle = new DeletableObject<>(construct(), UltralightMatrix::delete);
    }

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

    public native void set(UltralightMatrix4x4 ultralightMatrix4x4);

    private static native long construct();

    private static native void delete(long handle);

    @Override
    @NativeType("ultralight::Matrix *")
    public long getHandle() {
        return handle.get();
    }

    public native void setOrthographicProjection(@NativeType("double") double screenWidth, @NativeType(
            "double") double screenHeight, @NativeType("bool") boolean flipY);

    public native void transform(UltralightMatrix transformMat);

    public native UltralightMatrix4x4 getMatrix4x4();
}
