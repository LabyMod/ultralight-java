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

package net.labymedia.ultralight.javascript;

import net.labymedia.ultralight.annotation.NativeCall;

/**
 * Exception bridged from Javascript. Handling these is difficult and should be done as soon as possible,
 * as the contained exception value gets lost as soon as the context unlocks.
 * <p>
 * As Javascript exceptions, this exception is unchecked since it is not known when it can occur.
 */
public class JavascriptException extends RuntimeException {
    private final JavascriptValue value;

    /**
     * Constructs a new {@link JavascriptException}
     * @param message The message describing when the exception occurred
     * @param value The value that has been thrown by Javascript
     */
    @NativeCall
    public JavascriptException(String message, JavascriptValue value) {
        super(message);
        this.value = value;
    }

    /**
     * Retrieves the Javascript native value of this exception.
     *
     * @return The Javascript native value of this exception
     */
    public JavascriptValue getValue() {
        return value;
    }
}
