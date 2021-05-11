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
 * Callback for the initialization of Javascript objects.
 */
@NativeType("JSObjectInitializeCallback")
public interface JavascriptObjectInitializer {
    /**
     * The callback invoked when an object is first created.
     * <p>
     * Unlike the other object callbacks, the initialize callback is called on the least
     * derived class (the parent class) first, and the most derived class last.
     * <p>
     * <b>The parameters are only valid for the duration of execution of this method!</b>
     *
     * @param context The execution context to use
     * @param object  The JSObject being created
     */
    @NativeCall
    void initializeJavascriptObject(JavascriptContext context, JavascriptObject object);
}
