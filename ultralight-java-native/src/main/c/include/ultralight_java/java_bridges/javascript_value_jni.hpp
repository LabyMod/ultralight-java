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

#pragma once

#include <jni.h>

namespace ultralight_java {
    /**
     * Class for interfacing with JSValueRef from java
     */
    class JavascriptValueJNI {
    public:
        /**
         * Protects the value and returns a JavascriptProtectedValue object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return The protected value
         */
        static jobject protect(JNIEnv *env, jobject java_instance);

        /**
         * Releases the reference.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         */
        static void context_unlocking(JNIEnv *env, jobject java_instance);

        /**
         * Retrieves the type of the reference.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return The type of this reference as a java object
         */
        static jobject get_type(JNIEnv *env, jobject java_instance);

        /**
         * Tests whether this reference is an undefined value.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return true if this reference is an undefined value, false otherwise
         */
        static bool is_undefined(JNIEnv *env, jobject java_instance);

        /**
         * Tests whether this reference is a null value.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return true if this reference is a null value, false otherwise
         */
        static bool is_null(JNIEnv *env, jobject java_instance);

        /**
         * Tests whether this reference is a boolean value.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return true if this reference is a boolean value, false otherwise
         */
        static bool is_boolean(JNIEnv *env, jobject java_instance);

        /**
         * Tests whether this reference is a number value.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return true if this reference is a number value, false otherwise
         */
        static bool is_number(JNIEnv *env, jobject java_instance);

        /**
         * Tests whether this reference is a string value.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return true if this reference is a string value, false otherwise
         */
        static bool is_string(JNIEnv *env, jobject java_instance);

        /**
         * Tests whether this reference is a symbol value.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return true if this reference is a symbol value, false otherwise
         */
        static bool is_symbol(JNIEnv *env, jobject java_instance);

        /**
         * Tests whether this reference is an object value.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return true if this reference is an object value, false otherwise
         */
        static bool is_object(JNIEnv *env, jobject java_instance);

        /**
         * Tests whether this reference is an instance of a given class.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @param java_class The class to test for
         * @return true if this reference is an instance of the given class, false otherwise
         */
        static bool is_of_class(JNIEnv *env, jobject java_instance, jobject java_class);

        /**
         * Tests whether this reference is an array value.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return true if this reference is an array value, false otherwise
         */
        static bool is_array(JNIEnv *env, jobject java_instance);

        /**
         * Tests whether this reference is a date value.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return true if this reference is a date value, false otherwise
         */
        static bool is_date(JNIEnv *env, jobject java_instance);

        /**
         * Retrieves the JSTypedArrayType of this reference.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return The typed array type of this reference
         */
        static jobject get_typed_array_type(JNIEnv *env, jobject java_instance);

        /**
         * Tests whether this reference is equal to another.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @param java_other The other instance of a JSValueRef
         * @return true if the reference are equal, false otherwise
         */
        static bool is_equal(JNIEnv *env, jobject java_instance, jobject java_other);

        /**
         * Tests whether this reference is strictly equal to another.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @param java_other The other instance of a JSValueRef
         * @return true if the reference are strictly equal, false otherwise
         */
        static bool is_strict_equal(JNIEnv *env, jobject java_instance, jobject java_other);

        /**
         * Tests whether this reference has been constructed by the given constructor.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @param java_constructor The constructor to test against
         * @return true if this reference has been constructed by the given constructor, false otherwise
         */
        static bool is_instance_of_constructor(JNIEnv *env, jobject java_instance, jobject java_constructor);

        /**
         * Converts this reference to a json string.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @param indentation The amount of spaces to use for indentation
         * @return This reference as a json string
         */
        static jstring to_json(JNIEnv *env, jobject java_instance, jshort indentation);

        /**
         * Converts this reference to a boolean.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return This reference as a boolean
         */
        static jboolean to_boolean(JNIEnv *env, jobject java_instance);

        /**
         * Converts this reference to a number.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return This reference as a double
         */
        static jdouble to_number(JNIEnv *env, jobject java_instance);

        /**
         * Converts this reference to a string.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return This reference as a string
         */
        static jstring to_string_copy(JNIEnv *env, jobject java_instance);

        /**
         * Converts this reference to an object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSValueRef
         * @return This reference as an object
         */
        static jobject to_object(JNIEnv *env, jobject java_instance);
    };
} // namespace ultralight_java