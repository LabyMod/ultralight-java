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
import com.labymedia.ultralight.javascript.JavascriptObject;

/**
 * Callback for the finalization of Javascript objects.
 */
@NativeType("JSObjectFinalizeCallback")
public interface JavascriptObjectFinalizer {
    /**
     * The callback invoked when an object is finalized (prepared for garbage collection). An object may be finalized on
     * any thread.
     * <p>
     * The finalize callback is called on the most derived class first, and the least derived class (the parent class)
     * last.
     * <p>
     * <b>Functions which may cause garbage collection or allocation of new objects must not be called from inside this
     * callback!</b>
     *
     * @param object The object being finalized
     */
    @NativeCall
    void finalizeJavascriptObject(JavascriptObject object);
}
