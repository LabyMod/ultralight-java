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

#include "ultralight_java/java_bridges/ultralight_matrix4x4_jni.hpp"

#include "ultralight_java/java_bridges/ultralight_ref_ptr_jni.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    jobject UltralightMatrix4x4JNI::create(JNIEnv *env, ultralight::Matrix4x4 matrix) {
        return env->NewObject(
            runtime.ultralight_matrix4x4.clazz,
            runtime.ultralight_matrix4x4.constructor,
            new ultralight::Matrix4x4(matrix));
    }

    jlong UltralightMatrix4x4JNI::construct(JNIEnv *, jclass) {
        return reinterpret_cast<jlong>(new ultralight::Matrix4x4());
    }

    jfloatArray UltralightMatrix4x4JNI::get_data(JNIEnv *env, jobject instance) {
        auto *matrix4x4 = reinterpret_cast<ultralight::Matrix4x4 *>(
            env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        return Util::create_float_array(env, 16, matrix4x4->data);
    }

    void UltralightMatrix4x4JNI::set_identity(JNIEnv *env, jobject instance) {
        auto *matrix4x4 = reinterpret_cast<ultralight::Matrix4x4 *>(
            env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        matrix4x4->SetIdentity();
    }

    void UltralightMatrix4x4JNI::_delete(JNIEnv *, jclass, jlong handle) {
        delete reinterpret_cast<ultralight::Matrix4x4 *>(handle);
    }
} // namespace ultralight_java