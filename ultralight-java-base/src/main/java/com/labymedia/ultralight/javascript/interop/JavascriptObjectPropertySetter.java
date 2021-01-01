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
import com.labymedia.ultralight.javascript.JavascriptValue;

/**
 * Callback for setting properties on Javascript objects.
 */
@NativeType("JSObjectSetPropertyCallback")
public interface JavascriptObjectPropertySetter {
    /**
     * The callback invoked when setting a property's value.
     * <p>
     * If this function returns false, the set request forwards to object's statically declared properties, then its
     * parent class chain (which includes the default object class).
     *
     * @param context      The execution context to use
     * @param object       The object on which to set the property's value
     * @param propertyName A string containing the name of the property to set
     * @param value        A value to use as the property's value
     * @return {@code true} if the property was set, otherwise {@code false}
     * @throws JavascriptInteropException If an error occurs while setting the property
     */
    @NativeCall
    boolean setJavascriptProperty(
            JavascriptContext context, JavascriptObject object, String propertyName, JavascriptValue value)
            throws JavascriptInteropException;
}
