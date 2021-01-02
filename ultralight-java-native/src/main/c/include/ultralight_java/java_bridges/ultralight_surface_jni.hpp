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
     * Class for interfacing with ultralight::Surface from java.
     */
    class UltralightSurfaceJNI {
    public:
        /**
         * Retrieves the width of this surface.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the surface
         * @return The width of this surface
         */
        static jlong width(JNIEnv *env, jobject instance);

        /**
         * Retrieves the height of this surface.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the surface
         * @return The height of this surface
         */
        static jlong height(JNIEnv *env, jobject instance);

        /**
         * Retrieves the number of bytes between rows of this surface
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the surface
         * @return The number of bytes between rows of this surface
         */
        static jlong rowBytes(JNIEnv *env, jobject instance);

        /**
         * Retrieves the number of bytes of this surface.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the surface
         * @return The number of bytes of this surface
         */
        static jlong size(JNIEnv *env, jobject instance);

        /**
         * Retrieves a byte buffer backing this surface.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the surface
         * @return The byte buffer backing this surface
         */
        static jobject lockPixels(JNIEnv *env, jobject instance);

        /**
         * Unlocks the pixel buffer of this surface.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the surface
         */
        static void unlockPixels(JNIEnv *env, jobject instance);

        /**
         * Resizes this surface.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the surface
         * @param width The new width of this surface
         * @param height The new height of this surface
         */
        static void resize(JNIEnv *env, jobject instance, jlong width, jlong height);

        /**
         * Sets the dirty bounds of this surface.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the surface
         * @param rect The new dirty bounds
         */
        static void setDirtyBounds(JNIEnv *env, jobject instance, jobject rect);

        /**
         * Retrieves the dirty bounds of this surface.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the surface
         * @return The dirty bounds of this surface
         */
        static jobject dirtyBounds(JNIEnv *env, jobject instance);

        /**
         * Clears the dirty bounds of this surface.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the surface
         */
        static void clearDirtyBounds(JNIEnv *env, jobject instance);
    };
}