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
import com.labymedia.ultralight.ffi.gc.DeletableObject;
import com.labymedia.ultralight.javascript.interop.*;

/**
 * Builder class for creating class definitions for Javascript classes. Used to produce {@link
 * JavascriptClass} instances.
 */
@NativeType("JSClassDefinition")
public class JavascriptClassDefinition implements ObjectWithHandle {
    private final DeletableObject<Long> handle;

    /**
     * Constructs a new {@link JavascriptClassDefinition} which is completely empty.
     */
    public JavascriptClassDefinition() {
        this.handle = new DeletableObject<>(createEmpty(), JavascriptClassDefinition::free);
    }

    /**
     * Sets the name of the class being defined.
     *
     * @param name The name of the class
     * @return this
     */
    public native JavascriptClassDefinition name(String name);

    /**
     * Sets the attributes of the class being defined.
     *
     * @param attributes The attributes of the class, must be an OR'ed combination of values from {@link
     *                   JavascriptClassAttributes} or 0
     * @return this
     */
    public native JavascriptClassDefinition attributes(int attributes);

    /**
     * Sets the parent class of the class being defined.
     *
     * @param parent The parent class of the class
     * @return this
     */
    public native JavascriptClassDefinition parentClass(JavascriptClass parent);

    /**
     * Adds a static value to the class being defined.
     *
     * @param name       The name of the value to add
     * @param getter     The callback to use for retrieving the value
     * @param setter     The callback to use for setting the value, or {@code null}, if the value is read only
     * @param attributes The attributes of the static value, must be an OR'ed combination of values from {@link
     *                   JavascriptPropertyAttributes} or 0
     * @return this
     * @throws IllegalArgumentException If setter is null, but attributes does not contain {@link
     *                                  JavascriptPropertyAttributes#READ_ONLY}
     */
    public native JavascriptClassDefinition staticValue(
            String name, JavascriptObjectPropertyGetter getter, JavascriptObjectPropertySetter setter, int attributes);

    /**
     * Adds a static function to the class being defined.
     *
     * @param name       The name of the function to add
     * @param function   The callback to use for invoking the function
     * @param attributes The attributes of the function
     * @return this
     */
    public native JavascriptClassDefinition staticFunction(
            String name, JavascriptObjectFunction function, int attributes);

    /**
     * Sets the initializer of the class being defined.
     *
     * @param initializer The initializer of the class
     * @return this
     */
    public native JavascriptClassDefinition onInitialize(JavascriptObjectInitializer initializer);

    /**
     * Sets the finalizer of the class being defined.
     *
     * @param finalizer The finalizer of the class
     * @return this
     */
    public native JavascriptClassDefinition onFinalize(JavascriptObjectFinalizer finalizer);

    /**
     * Sets the callback used when testing for properties on the object.
     *
     * @param tester The callback to set
     * @return this
     */
    public native JavascriptClassDefinition onHasProperty(JavascriptObjectHasPropertyTester tester);

    /**
     * Sets the callback used for retrieving values from the object.
     *
     * @param getter The callback for retrieving values
     * @return this
     */
    public native JavascriptClassDefinition onGetProperty(JavascriptObjectPropertyGetter getter);

    /**
     * Sets the callback used for setting values on the object.
     *
     * @param setter The callback for setting values
     * @return this
     */
    public native JavascriptClassDefinition onSetProperty(JavascriptObjectPropertySetter setter);

    /**
     * Sets the callback used for removing values from the object.
     *
     * @param deleter The deleter for removing values
     * @return this
     */
    public native JavascriptClassDefinition onDeleteProperty(JavascriptObjectPropertyDeleter deleter);

    /**
     * Sets the callback used for retrieving the names of all properties from the object.
     *
     * @param collector The collector for retrieving names
     * @return this
     */
    public native JavascriptClassDefinition onGetPropertyNames(JavascriptObjectPropertyNamesCollector collector);

    /**
     * Sets the callback used for invoking the object as a function.
     *
     * @param function The function to invoke
     * @return this
     */
    public native JavascriptClassDefinition onCallAsFunction(JavascriptObjectFunction function);

    /**
     * Sets the callback used for constructing the object using the {@code new} keyword.
     *
     * @param constructor The constructor to invoke
     * @return this
     */
    public native JavascriptClassDefinition onCallAsConstructor(JavascriptObjectConstructor constructor);

    /**
     * Sets the callback used for determining the result of an {@code instanceof} expression.
     *
     * @param tester The tester invoked for testing
     * @return this
     */
    public native JavascriptClassDefinition onHasInstance(JavascriptObjectHasInstanceTester tester);

    /**
     * Sets the callback used for converting the object to another type.
     *
     * @param converter The converter for converting the object
     * @return this
     */
    public native JavascriptClassDefinition onConvertToType(JavascriptObjectToTypeConverter converter);

    /**
     * Creates the class and freezes its attributes. This {@link JavascriptClassDefinition} can be altered after calling
     * this method without altering the baked class. In return this also means that this method can be called multiple
     * times instead of using multiple definition objects to safe a tiny amount of memory.
     *
     * @return The created class
     * @throws IllegalStateException If a required property has not been set
     */
    public native JavascriptClass bake();

    /**
     * Allocates an empty class definition.
     *
     * @return A handle to the allocated definition
     */
    private static native long createEmpty();

    /**
     * Frees a class definition.
     *
     * @param handle A handle to the definition to free
     */
    private static native void free(long handle);

    @Override
    public long getHandle() {
        return handle.get();
    }
}
