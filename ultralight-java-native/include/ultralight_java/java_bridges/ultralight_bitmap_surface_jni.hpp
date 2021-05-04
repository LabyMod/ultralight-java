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
     * Class for interfacing with ultralight::BitmapSurface from java.
     */
    class UltralightBitmapSurfaceJNI {
    public:
        /**
         * Retrieves the underlying Bitmap from this Bitmap surface.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap surface
         * @return The underlying Bitmap from this surface
         */
        static jobject bitmap(JNIEnv *env, jobject instance);
    };
}