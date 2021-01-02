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
    /**
     * Class for interfacing with ultralight::KeyEvent from java.
     */
    class UltralightKeyEventJNI {
    public:
        /**
         * Retrieves a key identifier from a virtual key code.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The class calling this method, should always be UltralightKeyEvent
         * @param virtual_key_code The virtual key code to convert
         * @return The identifier as a jstring
         */
        static jstring get_key_identifier_from_virtual_key_code(JNIEnv *env, jclass caller_class, jobject virtual_key_code);
    };
}