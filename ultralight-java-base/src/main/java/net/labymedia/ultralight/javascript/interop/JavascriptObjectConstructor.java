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
import net.labymedia.ultralight.javascript.JavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptObject;
import net.labymedia.ultralight.javascript.JavascriptValue;

/**
 * Callback for calling a Javascript object with the new operator.
 */
@NativeType("JSObjectCallAsConstructorCallback")
public interface JavascriptObjectConstructor {
    /**
     * The callback invoked when an object is used as a constructor in a 'new' expression.
     *
     * @param context     The execution context to use
     * @param constructor An object that is the constructor being called
     * @param arguments   An array of the  arguments passed to the function
     * @return An object that is the constructor's return value
     * @throws JavascriptInteropException If an error occurs while invoking the constructor
     */
    @NativeCall
    JavascriptObject callAsJavascriptConstructor(
            JavascriptContext context, JavascriptObject constructor, JavascriptValue[] arguments)
            throws JavascriptInteropException;
}
