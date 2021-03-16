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

package com.labymedia.ultralight.javascript;

import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.ffi.ObjectWithHandle;

/**
 * A JavaScript execution context. Holds the global object and other execution state.
 */
@NativeType("JSContextRef")
public class JavascriptContext implements ObjectWithHandle, JavascriptLockedObject {
    private final long handle;
    private final JavascriptContextLock lock;

    /**
     * Constructs a new {@link JavascriptContext} wrapping the given native handle.
     *
     * @param handle The native handle to this context
     * @param lock   The lock this context belongs to
     */
    protected JavascriptContext(@NativeType("JSGlobalContextRef") long handle, JavascriptContextLock lock) {
        this.handle = handle;
        this.lock = lock;
    }

    /**
     * Retrieves the global context for this context.
     *
     * @return The global context owning this context
     */
    public native JavascriptGlobalContext getGlobalContext();

    /**
     * Retrieves the global object for this context.
     *
     * @return The global object for this context
     */
    public native JavascriptObject getGlobalObject();

    /**
     * Creates a value of the {@link JavascriptType#UNDEFINED}.
     *
     * @return The newly created value
     */
    public native JavascriptValue makeUndefined();

    /**
     * Creates a value of the {@link JavascriptType#NULL}.
     *
     * @return The newly created value
     */
    public native JavascriptValue makeNull();

    /**
     * Creates a value of the {@link JavascriptType#BOOLEAN} with the specified value.
     *
     * @param value The value of the new Javascript boolean
     * @return The newly created value
     */
    public native JavascriptValue makeBoolean(boolean value);

    /**
     * Creates a value of the {@link JavascriptType#NUMBER} with the specified value.
     *
     * @param value The value of the new Javascript number
     * @return The newly created value
     */
    public native JavascriptValue makeNumber(double value);

    /**
     * Creates a value of the {@link JavascriptType#STRING} with the specified value.
     *
     * @param value The value of the new Javascript string
     * @return The newly created value
     */
    public native JavascriptValue makeString(String value);

    /**
     * Creates a value of the {@link JavascriptType#SYMBOL} with the specified value.
     *
     * @param value The value of the new Javascript symbol
     * @return The newly created value
     */
    public native JavascriptValue makeSymbol(String value);

    /**
     * Creates a value with from the specified JSON snippet.
     *
     * @param data The JSON snippet to create the value from
     * @return The value created from the JSON, or {@code null} if the input is invalid
     */
    public native JavascriptValue makeFromJSON(String data);

    /**
     * Creates a new {@link JavascriptObject} without private data.
     *
     * @param clazz The Javascript class of the object
     * @return The newly created object
     */
    public JavascriptObject makeObject(JavascriptClass clazz) {
        return makeObject(clazz, null);
    }

    /**
     * Creates a new {@link JavascriptObject}.
     *
     * @param clazz       The Javascript class of the object
     * @param privateData The data to set as the private data on the object
     * @return The newly created object
     */
    public native JavascriptObject makeObject(JavascriptClass clazz, Object privateData);

    /**
     * Creates a new Javascript array.
     *
     * @param arguments The values to put into the array
     * @return The created array
     * @throws JavascriptException If the creation of the array fails
     */
    public native JavascriptObject makeArray(JavascriptValue... arguments) throws JavascriptException;

    /**
     * Creates a new Javascript date object.
     *
     * @param arguments The arguments to pass to the {@code Date} constructor
     * @return The created Date object
     * @throws JavascriptException If the creation of the object fails
     */
    public native JavascriptObject makeDate(JavascriptValue... arguments) throws JavascriptException;

    /**
     * Creates a new Javascript error object.
     *
     * @param arguments The arguments to pass to the {@code Error} constructor
     * @return The created Error object
     * @throws JavascriptException If the creation of the object fails
     */
    public native JavascriptObject makeError(JavascriptValue... arguments) throws JavascriptException;

    /**
     * Creates a new Javascript RegExp object.
     *
     * @param arguments The arguments to pass to the {@code RegExp} constructor
     * @return The created RegExp object
     * @throws JavascriptException If the creation of the object fails
     */
    public native JavascriptObject makeRegExp(JavascriptValue... arguments) throws JavascriptException;

    /**
     * Creates a function with a given script as its body.
     *
     * @param name               The name of the function, or {@code null}, to create an anonymous function
     * @param parameterNames     The names of the parameters passed to the function, or {@code null}, if none
     * @param body               The body of the function
     * @param sourceUrl          The URL where the function body originates from, used for exception reporting, can be
     *                           {@code null} to exclude this information
     * @param startingLineNumber The line number where the function has been declared, used for exception reporting,
     *                           values are clamped to 1 if they are invalid
     * @return The created function
     * @throws JavascriptException If the creation of the function fail
     */
    public native JavascriptObject makeFunction(
            String name,
            String[] parameterNames,
            String body,
            String sourceUrl,
            int startingLineNumber
    ) throws JavascriptException;

    /**
     * Performs Javascript garbage collection.
     * <p>
     * In theory, there is no need to call this manually while the Javascript engine is running. However, if you are
     * converting many Java objects to Javascript objects it is recommended to call this from time to time in order to
     * not block the JVM garbage collector.
     */
    public native void garbageCollect();

    @Override
    public void contextUnlocking() {
    }

    @Override
    public JavascriptContextLock getLock() {
        return lock;
    }

    @Override
    public long getContextHandle() {
        return lock.getContext().getHandle();
    }

    @Override
    public long getLockHandle() {
        return lock.getHandle();
    }

    @Override
    public long getHandle() {
        if (!lock.isLocked()) {
            throw new IllegalStateException("JavascriptContext is not locked anymore");
        }

        return handle;
    }
}
