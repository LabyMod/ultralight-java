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
        static void set1(JNIEnv *env, jobject instance, jobject matrix4x4);

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

        static jobject getMatrix4x4(JNIEnv *env, jobject instance);

        static jobject create(JNIEnv *env, ultralight::Matrix matrix);

        static jlong construct(JNIEnv *env, jclass caller_class);

        static void setOrthographicProjection(
            JNIEnv *env, jobject instance, jdouble width, jdouble height, jboolean flipY);

        static void transform(JNIEnv *env, jobject instance, jobject transformMatrix);

        static void _delete(JNIEnv *env, jclass caller_class, jlong handle);
    };
} // namespace ultralight_java