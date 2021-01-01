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
import com.labymedia.ultralight.ffi.ObjectWithHandle;
import com.labymedia.ultralight.ffi.gc.DeletableObject;

/**
 * A JavaScript class. Used with {@link JavascriptContext#makeObject(JavascriptClass, Object)}
 * to construct objects with custom behavior.
 */
@NativeType("JSClassRef")
public class JavascriptClass implements ObjectWithHandle {
    private final DeletableObject<Long> handle;

    /**
     * Constructs a new {@link JavascriptClass} wrapping an existing native handle.
     *
     * @param handle The native handle to wrap
     */
    private JavascriptClass(long handle) {
        this.handle = new DeletableObject<>(handle, JavascriptClass::release);
    }

    /**
     * Releases the class by decrementing its reference count.
     *
     * @param handle The native handle to decrement the reference count of
     */
    private static native void release(long handle);

    @Override
    public long getHandle() {
        return handle.get();
    }
}
