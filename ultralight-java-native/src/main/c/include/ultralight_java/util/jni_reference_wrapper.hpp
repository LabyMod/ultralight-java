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
     * Wrapper for JNI references with auto deletion
     */
    class JNIReferenceWrapper {
    protected:
        jobject reference;

    public:
        // Delete copy constructor and copy assignment operator
        JNIReferenceWrapper(const JNIReferenceWrapper &other) = delete;

        JNIReferenceWrapper &operator=(const JNIReferenceWrapper &other) = delete;

        // Implement move constructor, but delete move assignment operator
        JNIReferenceWrapper(JNIReferenceWrapper &&other) noexcept;

        JNIReferenceWrapper &operator=(JNIReferenceWrapper &&other) = delete;

        /**
         * Constructs a new JNIReferenceWrapper wrapping a jobject.
         *
         * @param env The environment to use for wrapping the reference
         * @param reference The reference to wrap, a global ref will be created for it
         */
        explicit JNIReferenceWrapper(JNIEnv *env, jobject reference);

        /**
         * Destructs the JNIReferenceWrapper and deletes the underlying reference.
         */
        virtual ~JNIReferenceWrapper();

        /**
         * Retrieves the wrapped reference.
         *
         * @return The wrapped reference
         */
        operator jobject();

        /**
         * Retrieves the wrapped reference.
         *
         * @return The wrapped reference
         */
        jobject operator*();

        /**
         * Retrieves the wrapped reference.
         *
         * @return The wrapped reference
         */
        jobject get();
    };
}