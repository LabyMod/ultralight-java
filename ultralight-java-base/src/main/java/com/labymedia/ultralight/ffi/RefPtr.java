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

package com.labymedia.ultralight.ffi;

import com.labymedia.ultralight.annotation.NativeCall;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.annotation.Unsigned;
import com.labymedia.ultralight.ffi.gc.DeletableObject;

/**
 * Wrapper for Ultralight smart pointers. Under the hood this RefPtr will only ever have one reference
 * on the java side, regardless of how many times it is copied. The underlying FFI garbage collector is
 * used to count these objects internally.
 *
 * @param <T> The type this pointer contains
 */
@NativeType("ultralight::RefPtr")
public class RefPtr<T> implements ObjectWithHandle {
    private final DeletableObject<Long> handle;

    /**
     * Constructs a new {@link RefPtr} and begins tracking the underlying handle.
     *
     * @param handle A pointer to a native `ultralight::RefPtr`, the java object will take
     *               ownership of the native pointer
     */
    @NativeCall
    private RefPtr(@NativeType("ultralight::RefPtr *") @Unsigned long handle) {
        this.handle = new DeletableObject<>(handle, RefPtr::delete);
    }

    @Override
    public @NativeType("ultralight::RefPtr *")
    @Unsigned
    long getHandle() {
        return handle.get();
    }

    /**
     * Executes the deletion of the native `ultralight::RefPtr` instance.
     * This method is static to not keep a reference to the java object, which
     * else would prevent deletion.
     *
     * @param handle A pointer to the instance to delete
     */
    private static native void delete(long handle);
}
