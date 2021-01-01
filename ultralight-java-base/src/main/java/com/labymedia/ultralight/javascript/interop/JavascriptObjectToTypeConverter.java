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
 * Callback for converting a Javascript object to another type.
 */
@NativeType("JSObjectConvertToTypeCallback")
public interface JavascriptObjectToTypeConverter {
    /**
     * The callback invoked when converting an object to a particular JavaScript type.
     *
     * @param context The execution context to use
     * @param object  The object to convert
     * @param type    A {@link JavascriptType} specifying the JavaScript type to convert to
     * @return The object's converted value, must not be null
     * @throws JavascriptInteropException If an error occurs while converting the object
     */
    @NativeCall
    JavascriptValue convertToJavascriptType(JavascriptContext context, JavascriptObject object, JavascriptType type)
            throws JavascriptInteropException;
}
