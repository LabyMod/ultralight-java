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
    class UltralightMatrixJNI {
    public:
        /**
         * Creates a new java object wrapping an existing Ultralight matrix.
         *
         * @param env The JNI environment to use for accessing java
         * @param matrix The matrix to wrap
         * @return The created java object
         */
        static jobject create(JNIEnv *env, ultralight::Matrix matrix);

        /**
         * Constructs a new Ultralight matrix pointer from java.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The class that called the method
         * @return The constructed matrix as a pointer
         */
        static jlong construct(JNIEnv *env, jclass caller_class);

        /**
         * Mutates the matrix by copying from a 4x4 matrix.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Matrix
         * @param matrix4x4 The java 4x4 matrix to copy from
         */
        static void set1(JNIEnv *env, jobject instance, jobject matrix4x4);

        /**
         * Mutates the matrix by setting its values.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Matrix
         * @param m11 The value at row 1, column 1
         * @param m12 The value at row 1, column 2
         * @param m13 The value at row 1, column 3
         * @param m14 The value at row 1, column 4
         * @param m21 The value at row 2, column 1
         * @param m22 The value at row 2, column 2
         * @param m23 The value at row 2, column 3
         * @param m24 The value at row 2, column 4
         * @param m31 The value at row 3, column 1
         * @param m32 The value at row 3, column 2
         * @param m33 The value at row 3, column 3
         * @param m34 The value at row 3, column 4
         * @param m41 The value at row 4, column 1
         * @param m42 The value at row 4, column 2
         * @param m43 The value at row 4, column 3
         * @param m44 The value at row 4, column 4
         */
        static void set(
            JNIEnv *env,
            jobject instance,
            jdouble m11,
            jdouble m12,
            jdouble m13,
            jdouble m14,
            jdouble m21,
            jdouble m22,
            jdouble m23,
            jdouble m24,
            jdouble m31,
            jdouble m32,
            jdouble m33,
            jdouble m34,
            jdouble m41,
            jdouble m42,
            jdouble m43,
            jdouble m44);

        /**
         * Retrieves this matrix as a 4x4 matrix.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Matrix
         * @return The java instance of the created 4x4 matrix
         */
        static jobject get_matrix_4x4(JNIEnv *env, jobject instance);

        /**
         * Mutates this matrix for orthographic projection.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Matrix
         * @param width The width of the projection
         * @param height The height of the projection
         * @param flip_y Whether the Y axis should be flipped
         */
        static void set_orthographic_projection(
            JNIEnv *env, jobject instance, jdouble width, jdouble height, jboolean flip_y);

        /**
         * Transforms this matrix with another matrix.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Matrix
         * @param transform_matrix The matrix to transform with
         */
        static void transform(JNIEnv *env, jobject instance, jobject transform_matrix);

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