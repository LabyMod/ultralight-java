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

/**
 * A java object which submits itself to a garbage collection queue to invoke deletion operations.
 *
 * @param <T> The type of the object which will be deleted by an {@link ObjectDeleter}.
 */
public class DeletableObject<T> {
    private final T value;
    private final BoundDeleter<T> deleter;

    /**
     * Constructs a new {@link DeletableObject} with a {@link ObjectDeleter} which will be notified when this object
     * will be deleted.
     *
     * @param value   The deletable value
     * @param deleter The deleter to invoke when deleting the value
     */
    public DeletableObject(T value, ObjectDeleter<T> deleter) {
        this.value = value;
        this.deleter = new BoundDeleter<>(value, deleter);

        // Submit the new object to the garbage collector
        FFIGarbageCollectorController.submit(this, this.deleter);
    }

    /**
     * Immediately deletes this object. Accessing the object after this method has been called is illegal.
     *
     * @return {@code true} if the object has been deleted, {@code false} otherwise
     */
    public boolean delete() {
        return deleter.delete();
    }

    /**
     * Determines if this object has been deleted already. This will only ever return {@code true} if the object has
     * been deleted by the user, as else the object would have only been deleted by garbage collection, at which point
     * it would not be accessible anymore.
     *
     * @return {@code true} if the object has been deleted already, {@code false} otherwise
     */
    public boolean isDeleted() {
        return deleter.isDeleted();
    }

    /**
     * Retrieves the stored value of this {@link DeletableObject}. The value needs to not be kept around or heap
     * corruption may occur when the original deletable object goes out of scope.
     *
     * @return The stored value
     */
    public T get() {
        return value;
    }
}
