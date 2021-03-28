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

@NativeType("ultralight::Matrix4x4")
public class UltralightMatrix4x4 implements ObjectWithHandle {

    private final DeletableObject<Long> handle;

    @NativeCall
    private UltralightMatrix4x4(long handle) {
        this.handle = new DeletableObject<>(handle, UltralightMatrix4x4::delete);
    }

    public UltralightMatrix4x4() {
        this.handle = new DeletableObject<>(construct(), UltralightMatrix4x4::delete);
    }

    public native @NativeType("float*") float[] getData();

    public native void setIdentity();

    private static native long construct();

    private static native void delete(long handle);

    @Override
    public long getHandle() {
        return this.handle.get();
    }
}
