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

/**
 * Callback for retrieving names of a Javascript object.
 */
@NativeType("JSObjectGetPropertyNamesCallback")
public interface JavascriptObjectPropertyNamesCollector {
    /**
     * The callback invoked when collecting the names of an object's properties.
     * <p>
     * {@link JavascriptObject#copyPropertyNames()} and JavaScript for...in loops.
     * <p>
     * A class's {@link JavascriptObjectPropertyNamesCollector} callback only needs to provide the names of properties
     * that the class vends through a custom {@link JavascriptObjectPropertyGetter} or {@link
     * JavascriptObjectPropertySetter}. Other properties, including statically declared properties, properties vended by
     * other classes, and properties belonging to object's prototype, are added independently.
     *
     * @param context The execution context to use
     * @param object  The object whose property names are being collected
     * @return An array of all property names on the given object
     */
    @NativeCall
    String[] collectJavascriptPropertyNames(JavascriptContext context, JavascriptObject object);
}
