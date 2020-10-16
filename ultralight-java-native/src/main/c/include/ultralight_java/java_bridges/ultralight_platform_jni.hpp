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

#include <jni.h>

namespace ultralight_java {
    /**
     * Class used for interfacing with ultralight::Platform from java.
     */
    class UltralightPlatformJNI {
    private:
        // Global ref to a java instance object
        static jobject global_instance;

    public:
        /**
         * Retrieves the java object matching the platform instance object.
         * If the instance does not exist yet, it is created.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The java class calling the method, should always be UltralightPlatform
         * @return The java platform instance object
         */
        static jobject instance(JNIEnv *env, jclass caller_class);

        /**
         * Sets the platform configuration object from the java object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The instance of the platform as a java object
         * @param java_config The new configuration as a java object
         */
        static void set_config(JNIEnv *env, jobject java_instance, jobject java_config);

        /**
         * Sets the font loader to the platform dependent implementation.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The instance of the platform as a java object
         */
        static void use_platform_font_loader(JNIEnv *env, jobject java_instance);

        /**
         * Sets the file system to the platform dependent implementation.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The instance of the platform as a java object
         * @param java_base_path The base path to resolve file:/// URL's relative to
         */
        static void use_platform_file_system(JNIEnv *env, jobject java_instance, jstring java_base_path);

        /**
         * Sets the internal file system to the provided java one.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The instance of the platform as a java object
         * @param java_file_system The java file system object
         */
        static void set_file_system(JNIEnv *env, jobject java_instance, jobject java_file_system);

        /**
         * Sets the internal clipboard to the provided java one
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The instance of the platform as a java object
         * @param java_clipboard The java clipboard object
         */
        static void set_clipboard(JNIEnv *env, jobject java_instance, jobject java_clipboard);

        /**
         * Sets the internal logger to the provided java one.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The instance of the platform as a java object
         * @param java_logger The java logger object
         */
        static void set_logger(JNIEnv *env, jobject java_instance, jobject java_logger);
    };
}