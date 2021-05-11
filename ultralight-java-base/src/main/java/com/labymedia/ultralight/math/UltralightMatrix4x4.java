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
 * 4x4 Matrix Helper.
 */
@NativeType("ultralight::Matrix4x4")
public class UltralightMatrix4x4 implements ObjectWithHandle {
    private final DeletableObject<Long> handle;

    /**
     * Constructs a new {@link UltralightMatrix4x4} and begins tracking the underlying handle.
     *
     * @param handle A pointer to a native ultralight::Matrix4x4, the java object will take
     *               ownership of the native pointer
     */
    @NativeCall
    private UltralightMatrix4x4(long handle) {
        this.handle = new DeletableObject<>(handle, UltralightMatrix4x4::delete);
    }

    /**
     * Constructs a new, empty 4x4 matrix.
     */
    public UltralightMatrix4x4() {
        this.handle = new DeletableObject<>(construct(), UltralightMatrix4x4::delete);
    }

    public native @NativeType("float*")
    float[] getData();

    /**
     * Set to identity matrix.
     */
    public native void setIdentity();

    /**
     * Constructs a native instance of the matrix.
     *
     * @return A pointer to the crated instance.
     */
    private static native long construct();

    /**
     * Executes the deletion of the native `ultralight::Matrix4x4` instance.
     * This method is static to not keep a reference to the java object, which
     * else would prevent deletion.
     *
     * @param handle A pointer to the instance to delete
     */
    private static native void delete(long handle);

    @Override
    public long getHandle() {
        return this.handle.get();
    }
}
