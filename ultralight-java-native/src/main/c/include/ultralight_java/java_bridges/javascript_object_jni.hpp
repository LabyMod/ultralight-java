/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
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

#pragma once

#include <jni.h>

namespace ultralight_java {
    /**
     * Class for interfacing with JSObjectRef from java
     */
    class JavascriptObjectJNI {
    public:
        /**
         * Retrieves the prototype of this object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @return The prototype as a java object
         */
        static jobject get_prototype(JNIEnv *env, jobject java_instance);

        /**
         * Sets the prototype of this object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param java_prototype The new prototype to set
         */
        static void set_prototype(JNIEnv *env, jobject java_instance, jobject java_prototype);

        /**
         * Determines whether this object has a property.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param java_property_name The name of the property to check for
         * @return true if this object has a property with the given name, false otherwise
         */
        static jboolean has_property(JNIEnv *env, jobject java_instance, jstring java_property_name);

        /**
         * Retrieves a property of this object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param java_property_name The name of the property to retrieve
         * @return The value of the property as a java object
         */
        static jobject get_property(JNIEnv *env, jobject java_instance, jstring java_property_name);

        /**
         * Sets a property of this object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param java_property_name THe name of the property to set
         * @param java_value The new value of the property as a java object
         * @param attributes The new attributes of the property
         */
        static void set_property(
            JNIEnv *env, jobject java_instance, jstring java_property_name, jobject java_value, jint attributes);

        /**
         * Deletes a property of this object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param java_property_name The name of the property to delete
         * @return true if the deletion succeeded, false if it failed
         */
        static jboolean delete_property(JNIEnv *env, jobject java_instance, jstring java_property_name);

        /**
         * Determines whether this object has a property for the given key.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param java_property_key The key of the property to test for
         * @return true if a property with such key was found, false otherwise
         */
        static jboolean has_property_for_key(JNIEnv *env, jobject java_instance, jobject java_property_key);

        /**
         * Retrieves a property of this object using the given key.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param java_property_key The key of the property to retrieve
         * @return The value of the property as a java object
         */
        static jobject get_property_for_key(JNIEnv *env, jobject java_instance, jobject java_property_key);

        /**
         * Sets a property of this object using the given key.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param java_property_key The key of the property to set
         * @param java_value The new value of the property
         * @param attributes The new attributes of the property
         */
        static void set_property_for_key(
            JNIEnv *env, jobject java_instance, jobject java_property_key, jobject java_value, jint attributes);

        /**
         * Deletes a property of this object using the given key.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param java_property_key The key of the property to delete
         * @return true if the deletion succeeded, false otherwise
         */
        static jboolean delete_property_for_key(JNIEnv *env, jobject java_instance, jobject java_property_key);

        /**
         * Retrieves a property of this object using the given index.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param property_index The index of the property to retrieve
         * @return The value of the property as a java object
         */
        static jobject get_property_at_index(JNIEnv *env, jobject java_instance, jlong property_index);

        /**
         * Sets a property of this object using the given index.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param property_index The index of the property to set
         * @param java_value The new value of the property
         */
        static void set_property_at_index(JNIEnv *env, jobject java_instance, jlong property_index, jobject java_value);

        /**
         * Retrieves the private data of this object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @return The private data of this object
         */
        static jobject get_private(JNIEnv *env, jobject java_instance);

        /**
         * Sets the private data of this object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param java_data The new private data of this object
         */
        static void set_private(JNIEnv *env, jobject java_instance, jobject java_data);

        /**
         * Determines whether this object can be called as a function.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @return true if this object can be called as a function, false otherwise
         */
        static jboolean is_function(JNIEnv *env, jobject java_instance);

        /**
         * Calls this object as a function.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param java_this_object The this object to pass to the invocation
         * @param java_arguments The arguments to call the function
         * @return The return value of the function as a java object
         */
        static jobject call_as_function(
            JNIEnv *env, jobject java_instance, jobject java_this_object, jobjectArray java_arguments);

        /**
         * Determines whether this object can be called as a constructor.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @return true if this object can be called as a constructor, false otherwise
         */
        static jboolean is_constructor(JNIEnv *env, jobject java_instance);

        /**
         * Calls this object as a constructor.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @param arguments The arguments to pass to the constructor
         * @return The constructed object
         */
        static jobject call_as_constructor(JNIEnv *env, jobject java_instance, jobjectArray java_arguments);

        /**
         * Copies the name of the enumerable properties of this object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSObjectRef
         * @return An array of enumerable property names
         */
        static jobjectArray copy_property_names(JNIEnv *env, jobject java_instance);
    };
} // namespace ultralight_java