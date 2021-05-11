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

package com.labymedia.ultralight.javascript.interop;

import com.labymedia.ultralight.annotation.NativeCall;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.javascript.JavascriptContext;
import com.labymedia.ultralight.javascript.JavascriptObject;
import com.labymedia.ultralight.javascript.JavascriptType;
import com.labymedia.ultralight.javascript.JavascriptValue;

/**
 * Callback for retrieving properties from a Javascript object.
 */
@NativeType("JSObjectGetPropertyCallback")
public interface JavascriptObjectPropertyGetter {
    /**
     * The callback invoked when getting a property's value.
     * <p>
     * If this function returns {@code null}, the get request forwards to object's statically declared properties, then
     * its parent class chain (which includes the default object class), then its prototype chain.
     *
     * @param context      The execution context to use
     * @param object       The object to search for the property
     * @param propertyName A string containing the name of the property to get
     * @return The property's value if object has the property, otherwise {@code null}, to return the literal null
     * value, construct a {@link JavascriptValue} with the {@link JavascriptType#NULL}
     * type
     * @throws JavascriptInteropException If an error occurs while retrieving the property
     */
    @NativeCall
    JavascriptValue getJavascriptProperty(JavascriptContext context, JavascriptObject object, String propertyName)
            throws JavascriptInteropException;
}
