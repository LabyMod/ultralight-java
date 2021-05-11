/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2021 LabyMedia and contributors
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
     * Class for interfacing with JSContextRef from java.
     */
    class JavascriptContextJNI {
    public:
        /**
         * Retrieves the global context as a java object for this context.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @return The java object for the global context of this context
         */
        static jobject get_global_context(JNIEnv *env, jobject java_instance);

        /**
         * Retrieves the global object for this context.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @return The global object for this context as a java object
         */
        static jobject get_global_object(JNIEnv *env, jobject java_instance);

        /**
         * Creates a new javascript value of the undefined type.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @return The created value
         */
        static jobject make_undefined(JNIEnv *env, jobject java_instance);

        /**
         * Creates a new javascript value of the null type.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @return The created value
         */
        static jobject make_null(JNIEnv *env, jobject java_instance);

        /**
         * Creates a new javascript value of the boolean type.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @param value The value of the boolean to create
         * @return The created value
         */
        static jobject make_boolean(JNIEnv *env, jobject java_instance, jboolean value);

        /**
         * Creates a new javascript value of the number type.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @param value The value of the number to create
         * @return The created value
         */
        static jobject make_number(JNIEnv *env, jobject java_instance, jdouble value);

        /**
         * Creates a new javascript value of the string type.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @param java_value The value of the string to create
         * @return The created value
         */
        static jobject make_string(JNIEnv *env, jobject java_instance, jstring java_value);

        /**
         * Creates a new javascript value of the symbol type.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @param java_value The value of the symbol to create
         * @return The created value
         */
        static jobject make_symbol(JNIEnv *env, jobject java_instance, jstring java_value);

        /**
         * Creates a new javascript value from a JSON snippet.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @param java_data The JSON data to create the value from
         * @return The created value
         */
        static jobject make_from_json(JNIEnv *env, jobject java_instance, jstring java_data);

        /**
         * Creates a new javascript object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @param java_javascript_class The class of the javascript object
         * @param private_data The data to set on the object
         * @return The created object
         */
        static jobject make_object(
            JNIEnv *env, jobject java_instance, jobject java_javascript_class, jobject private_data);

        /**
         * Creates a new javascript array.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @param java_arguments The values the array should contain
         * @return The created array
         */
        static jobject make_array(JNIEnv *env, jobject java_instance, jobjectArray java_arguments);

        /**
         * Creates a new javascript Date.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @param java_arguments The arguments to pass to the Date constructor
         * @return The created date
         */
        static jobject make_date(JNIEnv *env, jobject java_instance, jobjectArray java_arguments);

        /**
         * Creates a new javascript Error.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @param java_arguments The arguments to pass to the Error constructor
         * @return The created error
         */
        static jobject make_error(JNIEnv *env, jobject java_instance, jobjectArray java_arguments);

        /**
         * Creates a new javascript RegExp.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @param java_arguments The arguments to pass to the RegExp constructor
         * @return The created RegExp
         */
        static jobject make_reg_exp(JNIEnv *env, jobject java_instance, jobjectArray java_arguments);

        /**
         * Creates a javascript function from source code.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @param java_name The name of the function
         * @param java_parameter_names The name of the function parameters
         * @param java_body The script body of the function
         * @param java_source_url The source url the function originates from
         * @param starting_line_number The line number the function has been declared on
         * @return The created function
         */
        static jobject make_function(
            JNIEnv *env,
            jobject java_instance,
            jstring java_name,
            jobjectArray java_parameter_names,
            jstring java_body,
            jstring java_source_url,
            jint starting_line_number);

        /**
         * Performs Javascript garbage collection on the context.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         */
        static void garbage_collect(JNIEnv *env, jobject java_instance);
    };
} // namespace ultralight_java