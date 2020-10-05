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
     * Class for interfacing with JSGlobalContextRef from java.
     */
    class JavascriptGlobalContextJNI {
    public:
        /**
         * Releases the underlying JSContextRef instance.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this global context
         */
        static void context_unlocking(JNIEnv *env, jobject java_instance);

        /**
         * Retrieves the name of this context.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this global context
         * @return The name of this global context as a java string
         */
        static jstring get_name(JNIEnv *env, jobject java_instance);

        /**
         * Sets the name of this context.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this global context
         * @param java_name The new name of this context
         */
        static void set_name(JNIEnv *env, jobject java_instance, jstring java_name);
    };
}