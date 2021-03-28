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

#include "ultralight_java/java_bridges/ultralight_matrix_jni.hpp"

#include "ultralight_java/java_bridges/ultralight_matrix4x4_jni.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    jobject UltralightMatrixJNI::create(JNIEnv *env, ultralight::Matrix matrix) {
        return env->NewObject(
            runtime.ultralight_matrix.clazz, runtime.ultralight_matrix.constructor, new ultralight::Matrix(matrix));
    }

    jlong UltralightMatrixJNI::construct(JNIEnv *, jclass) {
        return reinterpret_cast<jlong>(new ultralight::Matrix());
    }

    void UltralightMatrixJNI::set1(JNIEnv *env, jobject instance, jobject target) {
        auto *matrix = reinterpret_cast<ultralight::Matrix *>(
            env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));

        auto *matrix4x4 = reinterpret_cast<ultralight::Matrix4x4 *>(
            env->CallLongMethod(target, runtime.object_with_handle.get_handle_method));

        matrix->Set(*matrix4x4);
    }

    void UltralightMatrixJNI::set(
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
        jdouble m44) {

        auto *matrix = reinterpret_cast<ultralight::Matrix *>(
            env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        matrix->Set(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);
    }

    void UltralightMatrixJNI::set_orthographic_projection(
        JNIEnv *env, jobject instance, jdouble width, jdouble height, jboolean flip_y) {
        auto *matrix = reinterpret_cast<ultralight::Matrix *>(
            env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));

        matrix->SetOrthographicProjection(width, height, flip_y);
    }

    jobject UltralightMatrixJNI::get_matrix_4x4(JNIEnv *env, jobject instance) {
        auto *matrix = reinterpret_cast<ultralight::Matrix *>(
            env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        return UltralightMatrix4x4JNI::create(env, matrix->GetMatrix4x4());
    }

    void UltralightMatrixJNI::transform(JNIEnv *env, jobject instance, jobject transform_matrix) {
        auto *matrix = reinterpret_cast<ultralight::Matrix *>(
            env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        auto *targetMatrix = reinterpret_cast<ultralight::Matrix *>(
            env->CallLongMethod(transform_matrix, runtime.object_with_handle.get_handle_method));

        matrix->Transform(*targetMatrix);
    }

    void UltralightMatrixJNI::_delete(JNIEnv *, jclass, jlong handle) {
        delete reinterpret_cast<ultralight::Matrix *>(handle);
    }
} // namespace ultralight_java
