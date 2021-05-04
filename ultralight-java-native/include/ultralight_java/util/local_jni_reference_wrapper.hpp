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

#pragma once

#include <jni.h>

namespace ultralight_java {
    /**
     * Wrapper for JNI references with auto deletion
     */
    template <typename T>
    class LocalJNIReferenceWrapper {
    protected:
        T reference;
        JNIEnv *env;

    public:
        /**
         * Constructs a new object and turns it into a local reference.
         *
         * @tparam Args The type of the arguments passed to the JNI call
         * @param env The JNI environment to use for accessing java
         * @param clazz The class of the object to create
         * @param constructor The id of the constructor to invoke
         * @param args The arguments to pass to the constructor
         * @return The created object
         */
        template <typename... Args>
        static LocalJNIReferenceWrapper construct(JNIEnv *env, jclass clazz, jmethodID constructor, Args &&... args) {
            return LocalJNIReferenceWrapper(env, env->NewObject(clazz, constructor, std::forward<Args>(args)...));
        }

        /**
         * Copy constructor copying another reference.
         *
         * @param other The reference to copy
         */
        LocalJNIReferenceWrapper(const LocalJNIReferenceWrapper &other)
            : env(other.env), reference(reinterpret_cast<T>(env->NewLocalRef(other.reference))){};

        /**
         * Copy assignment operator copying from another reference.
         *
         * @param other The reference to copy from
         * @return this
         */
        LocalJNIReferenceWrapper &operator=(const LocalJNIReferenceWrapper &other) {
            if(this == &other) {
                return *this;
            }

            if(reference) {
                env->DeleteLocalRef(reference);
            }

            env = other.env;
            reference = reinterpret_cast<T>(env->NewLocalRef(other.reference));
            return *this;
        }

        /**
         * Move constructor moving from another reference.
         *
         * @param other The reference to take ownership of
         */
        LocalJNIReferenceWrapper(LocalJNIReferenceWrapper &&other) noexcept
            : env(other.env), reference(other.reference) {
            other.reference = nullptr;
        }

        /**
         * Move assignment operator moving from another reference.
         *
         * @param other The reference to move from
         * @return this
         */
        LocalJNIReferenceWrapper &operator=(LocalJNIReferenceWrapper &&other) noexcept {
            if(reference) {
                env->DeleteLocalRef(reference);
            }

            env = other.env;
            reference = other.reference;

            other.reference = nullptr;
            return *this;
        }

        /**
         * Constructs a new JNIReferenceWrapper wrapping a null pointer.
         *
         * @param env THe environment to use for wrapping the null pointer
         */
        explicit LocalJNIReferenceWrapper(JNIEnv *env) : env(env), reference(nullptr) {}

        /**
         * Constructs a new JNIReferenceWrapper wrapping a jobject.
         *
         * @param env The environment to use for wrapping the reference
         * @param reference The reference to wrap, no new reference will be created
         */
        explicit LocalJNIReferenceWrapper(JNIEnv *env, T reference) : env(env), reference(reference) {
        }

        /**
         * Destructs the JNIReferenceWrapper and deletes the underlying reference.
         */
        virtual ~LocalJNIReferenceWrapper() {
            if(reference) {
                env->DeleteLocalRef(reference);
            }
        }

        /**
         * Retrieves the wrapped reference.
         *
         * @return The wrapped reference
         */
        operator T() {
            return reference;
        }

        /**
         * Retrieves the wrapped reference.
         *
         * @return The wrapped reference
         */
        T operator*() {
            return reference;
        }

        /**
         * Retrieves the wrapped reference.
         *
         * @return The wrapped reference
         */
        T get() {
            return reference;
        }

        /**
         * Tests whether this reference is not null.
         *
         * @return true if this reference is not null, false otherwise
         */
        operator bool() {
            return reference != nullptr;
        }
    };
} // namespace ultralight_java