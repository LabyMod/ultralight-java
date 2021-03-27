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
        static jfloatArray get_data(JNIEnv *env, jobject instance);
        static void set_identity(JNIEnv *env, jobject instance);
        static jobject create(JNIEnv *env, ultralight::Matrix4x4 matrix);
        static jlong construct(JNIEnv *env, jclass caller_class);
        static void _delete(JNIEnv *env, jclass caller_class, jlong handle);
    };
} // namespace ultralight_java