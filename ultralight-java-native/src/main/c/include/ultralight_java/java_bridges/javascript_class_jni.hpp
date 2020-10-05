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
#include "ultralight_java/platform/managed_javascript_class.hpp"

namespace ultralight_java {
    /**
     * Class for interfacing with JSClassRef from java
     */
    class JavascriptClassJNI {
    public:
        /**
         * Releases the data.
         * NOTE: This function is currently a NO-OP, because JavascriptCore does not properly track references,
         * and us freeing them would cause crashes!
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The java class calling the method, should always be JavascriptClass
         * @param handle The handle of the JSClassRef to release
         */
        static void release(JNIEnv *env, jclass caller_class, jlong handle);
    };
}