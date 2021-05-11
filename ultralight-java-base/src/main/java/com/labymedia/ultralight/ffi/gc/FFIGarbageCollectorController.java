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

package com.labymedia.ultralight.ffi.gc;

import com.labymedia.ultralight.ffi.gc.impl.DefaultFFIGarbageCollectorImpl;

/**
 * Garbage collector manager for FFI objects. Used in conjunction with
 * {@link DeletableObject}s.
 */
public class FFIGarbageCollectorController {
    private static FFIGarbageCollector collector;

    /**
     * Submits a new object for deletion with a given deleter.
     *
     * @param object  The object to listen to deletion for
     * @param deleter The deleter to run when the object gets garbage collected
     * @param <T>     The type to garbage collect
     */
    public static <T> void submit(DeletableObject<T> object, BoundDeleter<T> deleter) {
        if (collector == null) {
            // The user has not overwritten the collector
            collector = new DefaultFFIGarbageCollectorImpl();
        }

        collector.submit(object, deleter);
    }

    /**
     * Sets the global garbage collector instance used for tracking and deleting
     * FFI objects.
     * <p>
     * This may only be done if the collector has not been set already. The implementation
     * sets the default collector automatically when the first FFI garbage collected object
     * is created and no collector has been supplied by the user.
     *
     * @param collector The new garbage collector instance
     * @throws IllegalStateException If the collector has been set already
     */
    public static void setCollector(FFIGarbageCollector collector) {
        if (FFIGarbageCollectorController.collector != null) {
            throw new IllegalStateException("The garbage collector has been set already, " +
                    "try setting it earlier if this has not been done by you");
        }

        FFIGarbageCollectorController.collector = collector;
    }
}
