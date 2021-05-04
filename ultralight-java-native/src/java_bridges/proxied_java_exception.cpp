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

#include "ultralight_java/java_bridges/proxied_java_exception.hpp"

namespace ultralight_java {
    ProxiedJavaException::ProxiedJavaException(JNIReferenceWrapper thrown) : thrown(std::move(thrown)) {}

    void ProxiedJavaException::throw_if_any(JNIEnv *env) {
        if(!env->ExceptionCheck()) {
            // Can't throw anything
            return;
        } else {
            // Retrieve the current exception and remove it from the environment
            jthrowable thrown = env->ExceptionOccurred();
            env->ExceptionClear();

            // Create a proxied exception and throw it
            throw ProxiedJavaException(JNIReferenceWrapper(env, thrown));
        }
    }

    void ProxiedJavaException::throw_to_java(JNIEnv *env) {
        env->Throw(reinterpret_cast<jthrowable>(*thrown));
    }
}