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

import com.labymedia.ultralight.annotation.NativeCall;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.annotation.Unsigned;

@NativeType("JSObjectRef")
public class JavascriptObject extends JavascriptValue {
    /**
     * Constructs a new {@link JavascriptObject} wrapping an existing value reference in a locked context.
     *
     * @param handle The native value reference
     * @param lock   The context the value exists in
     */
    @NativeCall
    JavascriptObject(long handle, JavascriptContextLock lock) {
        super(handle, lock);
    }

    /**
     * Retrieves the prototype of the object.
     *
     * @return The prototype of the object
     */
    public native JavascriptValue getPrototype();

    /**
     * Sets the prototype of the object.
     *
     * @param prototype The new prototype
     */
    public native void setPrototype(JavascriptValue prototype);

    /**
     * Tests whether the object has the given property.
     *
     * @param propertyName The name of the property to test for
     * @return {@code true} if a property matching the given name exists, {@code false} otherwise
     */
    public native boolean hasProperty(String propertyName);

    /**
     * Retrieves a property of the object.
     *
     * @param propertyName The name of the property to retrieve
     * @return The value of the property if it exists, a {@link JavascriptType#UNDEFINED} value else
     * @throws JavascriptException If an error occurs while retrieving the property
     */
    public native JavascriptValue getProperty(String propertyName) throws JavascriptException;

    /**
     * Sets a property of the object.
     *
     * @param propertyName The name of the property to set
     * @param value        The new value of the property
     * @param attributes   A combination of {@link JavascriptPropertyAttributes} logically OR'ed together
     * @throws JavascriptException If an error occurs while setting the property
     */
    public native void setProperty(
            String propertyName,
            JavascriptValue value,
            @NativeType("JSPropertyAttributes") int attributes
    ) throws JavascriptException;

    /**
     * Deletes a property of the object.
     *
     * @param propertyName The name of the property to delete
     * @return {@code true} if the deletion succeeded, {@code false} otherwise
     * @throws JavascriptException If an error occurs while deleting the property
     */
    public native boolean deleteProperty(String propertyName) throws JavascriptException;

    /**
     * Tests whether the object has a property for the given property key.
     *
     * @param propertyKey The key of the property to check for
     * @return {@code true} if a property for the given key exists, {@code false} otherwise
     * @throws JavascriptException If an error occurs while checking for the property
     */
    public native boolean hasPropertyForKey(JavascriptValue propertyKey) throws JavascriptException;

    /**
     * Retrieves a property of the object using the given property key.
     *
     * @param propertyKey The key of the property to retrieve
     * @return The value of the property, or a {@link JavascriptType#UNDEFINED} value if the property does not exist
     * @throws JavascriptException If an error occurs while retrieving the property
     */
    public native JavascriptValue getPropertyForKey(JavascriptValue propertyKey) throws JavascriptException;

    /**
     * Sets a property of the object using the given property key.
     *
     * @param propertyKey The key of the property to set
     * @param value       The new value of the property
     * @param attributes  A combination of {@link JavascriptPropertyAttributes} logically OR'ed together
     * @throws JavascriptException If an error occurs while setting the property
     */
    public native void setPropertyForKey(
            JavascriptValue propertyKey,
            JavascriptValue value,
            int attributes
    ) throws JavascriptException;

    /**
     * Deletes a property of the object using the given property key.
     *
     * @param propertyKey The key of the property to delete
     * @return {@code true} if the deletion succeeded, {@code false} otherwise
     * @throws JavascriptException If an error occurs while deleting the property
     */
    public native boolean deletePropertyForKey(JavascriptValue propertyKey) throws JavascriptException;

    /**
     * Retrieves a property of the object using the given index.
     *
     * @param propertyIndex The index of the property to retrieve
     * @return The value of the property, or a {@link JavascriptType#UNDEFINED} value if the index is out of range
     * @throws JavascriptException If an error occurs while retrieving the property
     */
    public native JavascriptValue getPropertyAtIndex(
            @NativeType("unsigned") @Unsigned long propertyIndex) throws JavascriptException;

    /**
     * Sets a property of the object using the given index.
     *
     * @param propertyIndex The index of the property to set
     * @param value         The new value of the property
     * @throws JavascriptException If an error occurs while setting the property
     */
    public native void setPropertyAtIndex(
            @NativeType("unsigned") @Unsigned long propertyIndex, JavascriptValue value) throws JavascriptException;

    /**
     * Retrieves the private data of the object.
     *
     * @return The private data of the object, or {@code null} if no private data has been set
     */
    public native Object getPrivate();

    /**
     * Sets the private data of the object
     *
     * @param data The new private data of the object
     * @throws UnsupportedOperationException If the object can't store private data
     */
    public native void setPrivate(Object data) throws UnsupportedOperationException;

    /**
     * Tests whether the object can be called as a function.
     *
     * @return {@code true} if the object can be called as a function, {@code false} otherwise
     */
    public native boolean isFunction();

    /**
     * Calls the object as a function.
     *
     * @param thisObject The object to use as {@code this} during the call, or {@code null}, to use the global object
     * @param arguments  The arguments to pass to the function
     * @return The result of the invocation
     * @throws JavascriptException If an error occurs while invoking the function
     */
    public native JavascriptValue callAsFunction(
            JavascriptObject thisObject, JavascriptValue... arguments) throws JavascriptException;

    /**
     * Tests whether the object can be called as a constructor.
     *
     * @return {@code true} if the object can be called as a constructor, {@code false} otherwise
     */
    public native boolean isConstructor();

    /**
     * Calls the object as a constructor.
     *
     * @param arguments The arguments to pass to the constructor
     * @return The constructed object
     * @throws JavascriptException If an error occurs while invoking the constructor
     */
    public native JavascriptObject callAsConstructor(JavascriptValue... arguments) throws JavascriptException;

    /**
     * Retrieves the names of the objects enumerable properties.
     *
     * @return The object enumerable property names
     */
    public native String[] copyPropertyNames();
}
