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

package com.labymedia.ultralight.util;

/**
 * General purpose utility class containing methods which don't fit anywhere else.
 */
public class Util {
    // Static class
    private Util() {
    }

    /**
     * Method to simply force cast any type to any type. Use with caution,
     * absolutely no checks are done. This is often useful when generic signatures
     * begin to clash and you can ensure that it is safe to erase them.
     *
     * @param o   The object to cast
     * @param <T> The target type
     * @return The casted object
     */
    @SuppressWarnings("unchecked")
    public static <T> T forceCast(Object o) {
        return (T) o;
    }
}
