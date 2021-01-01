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
 * Attributes of properties on Javascript classes. Defines how those properties can be altered.
 */
public final class JavascriptPropertyAttributes {
    /**
     * Specifies that a property has no special attributes.
     */
    public static int NONE = 0;

    /**
     * Specifies that a property is read-only.
     */
    public static int READ_ONLY = 1 << 1;

    /**
     * <p>
     * Specifies that a property should not be enumerated by {@link JavascriptObject#copyPropertyNames()}
     * and JavaScript for...in loops.
     */
    public static int DONT_ENUM = 1 << 2;

    /**
     * Specifies that the delete operation should fail on a property.
     */
    public static int DONT_DELETE = 1 << 3;
}
