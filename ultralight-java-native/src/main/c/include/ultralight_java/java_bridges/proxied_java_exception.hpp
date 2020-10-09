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
#include <exception>

#include "ultralight_java/util/jni_reference_wrapper.hpp"

namespace ultralight_java {
    /**
     * C++ exception for a caught java exception.
     */
    class ProxiedJavaException : public std::exception {
    private:
        // The thrown object
        JNIReferenceWrapper thrown;

        /**
         * Constructs a new ProxiedJavaException with the given underlying throwable.
         *
         * @param thrown The thrown throwable
         */
        explicit ProxiedJavaException(JNIReferenceWrapper thrown);

    public:
        /**
         * Constructs a JavaProxiedException from the exception in the current JNI environment.
         * This method removes the exception from the environment. If there is no active exception,
         * then this is a no-op.
         *
         * @param env The environment to obtain the exception from
         * @throws ProxiedJavaException The currently active java exception
         */
        static void throw_if_any(JNIEnv *env);

        ProxiedJavaException(const ProxiedJavaException &other) = delete;

        /**
         * Throws this exception back to Java and clears the throwable.
         *
         * @param env The environment to throw the exception in
         */
        void throw_to_java(JNIEnv *env);
    };
}