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

#include <jni.h>

namespace ultralight_java {
    /**
     * Helper for obtaining a JNI environment for a limited amount of time.
     */
    class TemporaryJNI {
    private:
        // The stored environment
        JNIEnv *env;
        bool detach_on_destruct;

    public:
        /**
         * Constructs a new TemporaryJNI environment and attaches it to the JVM
         * if required.
         */
        explicit TemporaryJNI();

        /**
         * Destructs the temporary JNI environment, detaches it from the JVM
         * if it has been attached by the constructor.
         */
        ~TemporaryJNI();

        /**
         * Retrieves the JNI environment contained by this temporary environment.
         *
         * @return The contained JNI environment
         */
        operator JNIEnv *();

        /**
         * Retrieves the JNI environment contained by this temporary environment.
         *
         * @return The contained JNI environment
         */
        JNIEnv *operator*();

        /**
          * Retrieves the JNI environment contained by this temporary environment.
          *
          * @return The contained JNI environment
          */
        JNIEnv *operator->();
    };
}