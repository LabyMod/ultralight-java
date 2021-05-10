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

package com.labymedia.ultralight.databind.call.property;

import com.labymedia.ultralight.javascript.interop.JavascriptInteropException;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;

/**
 * Represents an abstraction about calling properties on java objects or classes.
 */
public interface PropertyCaller {

    /**
     * Calls a method on a certain instance.
     *
     * @param instance   The instance the method belongs to, or {@code null}, if static
     * @param method     The method which should be called
     * @param parameters The parameters the method should be called with
     * @return The return value of the method, or {@code null}, if void
     * @throws JavascriptInteropException If an error occurred while calling the method
     */
    Object callMethod(Object instance, Method method, Object[] parameters) throws JavascriptInteropException;

    /**
     * Creates a new instance of a class by calling a certain constructor.
     *
     * @param constructor The constructor which should be called
     * @param parameters  The parameters the constructor should be called with
     * @return The new instance
     * @throws JavascriptInteropException If an error occurred while calling the function
     */
    Object callConstructor(Constructor<?> constructor, Object[] parameters) throws JavascriptInteropException;

    /**
     * Returns the value from a certain field.
     *
     * @param instance The instance the field belongs to, or {@code null}, if static
     * @param field    The field whose value should be returned
     * @return The value of the field
     * @throws JavascriptInteropException If an error occurred while getting the value from the field
     */
    Object callFieldGet(Object instance, Field field) throws JavascriptInteropException;

    /**
     * Sets a value of a certain field.
     *
     * @param instance The instance the field belongs to, or {@code null}, if static
     * @param field    The field whose value should be set
     * @param value    The value to be set
     * @throws JavascriptInteropException If an error occurred while setting the value of the field
     */
    void callFieldSet(Object instance, Field field, Object value) throws JavascriptInteropException;

    /**
     * Factory for {@link PropertyCaller}.
     */
    interface Factory {

        /**
         * @return A new {@link PropertyCaller} instance
         */
        PropertyCaller create();
    }
}
