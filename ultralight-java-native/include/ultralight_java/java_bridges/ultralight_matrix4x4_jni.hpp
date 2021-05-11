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

#include <Ultralight/Ultralight.h>
#include <jni.h>

namespace ultralight_java {
    class UltralightMatrix4x4JNI {
    public:
        /**
         * Creates a new java object wrapping an existing Ultralight 4x4 matrix.
         *
         * @param env The JNI environment to use for accessing java
         * @param matrix The matrix to wrap
         * @return The created java object
         */
        static jobject create(JNIEnv *env, ultralight::Matrix4x4 matrix);

        /**
         * Constructs a new Ultralight matrix pointer from java.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The class that called the method
         * @return The constructed matrix as a pointer
         */
        static jlong construct(JNIEnv *env, jclass caller_class);

        /**
         * Retrieves the data of the matrix as a float array.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Matrix
         * @return The data of this matrix as a java float array
         */
        static jfloatArray get_data(JNIEnv *env, jobject instance);

        /**
         * Sets this matrix as an identity matrix.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Matrix
         */
        static void set_identity(JNIEnv *env, jobject instance);

        /**
         * Deletes the underlying pointer of the wrapper Matrix object.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Matrix
         * @param handle A pointer to the Matrix object to delete
         */
        static void _delete(JNIEnv *env, jclass caller_class, jlong handle);
    };
} // namespace ultralight_java