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

/**
 * A Javascript value that is currently not within any locked context. Those values keep references to their underlying
 * objects and prevent them from freeing. Don't keep these objects around forever, as this will create a memory leak
 * else.
 */
public class JavascriptProtectedValue {
    private long handle;

    /**
     * Constructs a new {@link JavascriptProtectedValue} wrapping the given native value reference. The native value
     * needs to be protected already!
     *
     * @param handle The native, already protected handle
     */
    JavascriptProtectedValue(long handle) {
        this.handle = handle;
    }

    /**
     * "revives" the value by attaching it to a lock again. This invalidates this value.
     *
     * @param lock The lock to attach the value to
     * @return The reattached value
     * @throws IllegalArgumentException If the given lock is null
     * @throws IllegalStateException    If the value has been revived already
     */
    public JavascriptValue revive(JavascriptContextLock lock) {
        if (lock == null) {
            throw new IllegalArgumentException(new NullPointerException("lock"));
        } else if (handle == 0) {
            throw new IllegalStateException("The value has been revived already");
        }

        long tHandle = handle;
        handle = 0;

        return new JavascriptValue(tHandle, lock);
    }
}
