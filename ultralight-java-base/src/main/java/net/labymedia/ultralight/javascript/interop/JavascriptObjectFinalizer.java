/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package net.labymedia.ultralight.javascript.interop;

import net.labymedia.ultralight.annotation.NativeCall;
import net.labymedia.ultralight.annotation.NativeType;
import net.labymedia.ultralight.javascript.JavascriptObject;

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
