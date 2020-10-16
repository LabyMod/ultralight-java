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

#include <functional>

#include <jni.h>
#include <Ultralight/Ultralight.h>

#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    /**
     * Wrapper for ultralight::RefPtr to erase their type signature
     * so they can be passed to java.
     */
    struct WrappedRefPtr {
        /**
         * Function deleting the value stored in ptr_value.
         */
        std::function<void(void *)> deleter;

        /**
         * Pointer to the ultralight::RefPtr itself.
         */
        void *ptr_value;
    };

    /**
     * Class used for interfacing with ultralight::RefPtr from java.
     */
    class UltralightRefPtrJNI {
    public:
        /**
         * Creates a new java ref pointer implementation from the
         * given ultralight::RefPtr.
         *
         * @tparam T The type of the reference counted object
         * @param env The JNI environment to use for accessing java
         * @param ptr The reference pointer to convert to a java object
         * @return The converted java object
         */
        template<typename T>
        static jobject create(JNIEnv *env, ultralight::RefPtr<T> ptr) {
            // Move the ref pointer to a heap location
            auto *moved_ptr = new ultralight::RefPtr<T>(std::move(ptr));

            // Pack the pointer with a deleter and its value
            auto *wrapped = new WrappedRefPtr{
                    std::function < void(void * ) > ([](void *to_delete) {
                        delete reinterpret_cast<ultralight::RefPtr<T> *>(to_delete);
                    }),
                    reinterpret_cast<void *>(moved_ptr)
            };

            return env->NewObject(runtime.ref_ptr.clazz, runtime.ref_ptr.constructor, reinterpret_cast<jlong>(wrapped));
        }

        /**
         * Unwraps the ultralight::RefPtr from a WrappedRefPtr.
         *
         * @tparam T The type the wrapper pointer contains
         * @param env The JNI environment to use for accessing java
         * @param object The java object containing the wrapped pointer as a handle
         * @return The unwrapped ultralight::RefPtr
         */
        template<typename T>
        static ultralight::RefPtr<T> unwrap_ref_ptr(JNIEnv *env, jobject object) {
            jlong handle = env->CallLongMethod(object, runtime.object_with_handle.get_handle_method);
            if(env->ExceptionCheck()) {
                return nullptr;
            }

            return unwrap_ref_ptr<T>(handle);
        }

        /**
         * Unwraps the ultralight::RefPtr from a WrappedRefPtr.
         *
         * @tparam T The type the wrapper pointer contains
         * @param handle A handle to a WrappedRefPtr struct
         * @return The unwrapped ultralight::RefPtr
         */
        template<typename T>
        static ultralight::RefPtr<T> unwrap_ref_ptr(jlong handle) {
            auto *wrapped = reinterpret_cast<WrappedRefPtr *>(handle);
            return *reinterpret_cast<ultralight::RefPtr<T> *>(wrapped->ptr_value);
        }

        /**
         * Deletes a ultralight::RefPtr by unwrapping it from the java object.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The caller class, should always be the RefPtr java class
         * @param handle A pointer wrapped as a long to a ultralight::RefPtr
         */
        static void _delete(JNIEnv *env, jclass caller_class, jlong handle);
    };
}