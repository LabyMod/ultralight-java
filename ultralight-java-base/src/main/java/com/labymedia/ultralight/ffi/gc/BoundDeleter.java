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
 * Bound version of a {@link ObjectDeleter}.
 */
public class BoundDeleter<T> {
    private final T value;
    private final ObjectDeleter<T> deleter;

    private boolean deleted;

    /**
     * Crates a new {@link BoundDeleter} binding an object and its deleter.
     *
     * @param value   The object to bind to the deleter
     * @param deleter The deleter to bind the object to
     */
    BoundDeleter(T value, ObjectDeleter<T> deleter) {
        this.value = value;
        this.deleter = deleter;
        this.deleted = false;
    }

    /**
     * Runs the deleter with the stored object.
     *
     * @return {@code true} if the object has been deleted, {@code false} otherwise
     */
    public boolean delete() {
        if (deleted) {
            return false;
        }

        deleter.delete(value);
        deleted = true;
        return true;
    }

    /**
     * Determines if this deleter has been executed already.
     *
     * @return {@code true} if the deleter has been executed already, {@code false} otherwise
     */
    public boolean isDeleted() {
        return deleted;
    }

    @Override
    public String toString() {
        return "BoundDeleter{" +
                "value=" + value +
                ", deleter=" + deleter +
                '}';
    }
}
