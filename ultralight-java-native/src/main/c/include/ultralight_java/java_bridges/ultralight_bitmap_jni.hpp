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
     * Class for interfacing with ultralight::Bitmap from java.
     */
    class UltralightBitmapJNI {
    public:
        /**
         * Creates a new empty Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The class calling this method, should always be UltralightBitmap
         * @return The created Bitmaps as a java object
         */
        static jobject create(JNIEnv *env, jclass caller_class);

        /**
         * Creates a new empty Bitmap with the given dimensions and format.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The class calling this method, should always be UltralightBitmap
         * @param width The width of the Bitmap in pixels
         * @param height The height of the Bitmap in pixels
         * @param format The format of the Bitmap as a java object
         * @return The created Bitmap as a java object
         */
        static jobject create(JNIEnv *env, jclass caller_class, jlong width, jlong height, jobject format);

        /**
         * Creates a new Bitmap from an existing pixel buffer.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The class calling this method, should always be UltralightBitmap
         * @param width The width of the Bitmap in pixels
         * @param height The height of the Bitmap in pixels
         * @param format The format of the Bitmap as a java object
         * @param row_bytes The bytes per each row
         * @param pixel_buffer The existing pixel buffer
         * @param fixup_gamma Whether the gamma data should be fixed up
         * @return The created Bitmap as a java object
         */
        static jobject create(
                JNIEnv *env,
                jclass caller_class,
                jlong width,
                jlong height,
                jobject format,
                jlong row_bytes,
                jobject pixel_buffer,
                jboolean fixup_gamma
        );

        /**
         * Creates a new Bitmap by copying another Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The class calling this method, should always be UltralightBitmap
         * @param bitmap The Bitmap to copy from
         * @return The copied Bitmap as a java object
         */
        static jobject create(JNIEnv *env, jclass caller_class, jobject bitmap);

        /**
         * Retrieves the width of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @return The width of this Bitmap
         */
        static jlong width(JNIEnv *env, jobject instance);

        /**
         * Retrieves the height of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @return The height of this Bitmap
         */
        static jlong height(JNIEnv *env, jobject instance);

        /**
         * Retrieves the bounds of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @return The bounds of this Bitmap
         */
        static jobject bounds(JNIEnv *env, jobject instance);

        /**
         * Retrieves the format of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @return The format of this Bitmap
         */
        static jobject format(JNIEnv *env, jobject instance);

        /**
         * Retrieves the bytes per pixel of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @return The bytes per pixel of this Bitmap
         */
        static jlong bpp(JNIEnv *env, jobject instance);

        /**
         * Retrieves the bytes per row of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @return The bytes per row of this Bitmap
         */
        static jlong row_bytes(JNIEnv *env, jobject instance);

        /**
         * Retrieves the size in bytes of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @return The size in bytes of this Bitmap
         */
        static jlong size(JNIEnv *env, jobject instance);

        /**
         * Checks if this Bitmaps owns the pixel buffer.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @return Whether this Bitmaps owns the pixel buffer
         */
        static jboolean owns_pixels(JNIEnv *env, jobject instance);

        /**
         * Locks and retrieves the pixel buffer of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @return The pixel buffer of this Bitmap as a java NIO byte buffer
         */
        static jobject lock_pixels(JNIEnv *env, jobject instance);

        /***
         * Unlocks the pixel buffer of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         */
        static void unlock_pixels(JNIEnv *env, jobject instance);

        /**
         * Retrieves the pixel buffer of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @return The pixel buffer of this Bitmap as a java NIO byte buffer
         */
        static jobject raw_pixels(JNIEnv *env, jobject instance);

        /**
         * Checks whether this Bitmap is empty (has no pixels allocated).
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @return Whether this Bitmap is empty
         */
        static jboolean is_emtpy(JNIEnv *env, jobject instance);

        /**
         * Erases the pixel data of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         */
        static void erase(JNIEnv *env, jobject instance);

        /**
         * Assigns another Bitmap to this Bitmap by copying from it.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @param other The Bitmap to copy from
         */
        static void set(JNIEnv *env, jobject instance, jobject other);

        /**
         * Draws another Bitmap to this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @param java_src_rect The source rectangle
         * @param java_dest_rect The destination rectangle
         * @param java_src The source Bitmap
         * @param pad_repeat Whether the edge pixels should be padded
         * @return Whether the operation succeeded
         */
        static jboolean draw_bitmap(
                JNIEnv *env,
                jobject instance,
                jobject java_src_rect,
                jobject java_dest_rect,
                jobject java_src,
                bool pad_repeat
        );

        /**
         * Writes this Bitmap as a PNG to disk.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @param java_path The path to write the PNG to
         * @return Whether this operation succeeded
         */
        static jboolean write_png(JNIEnv *env, jobject instance, jstring java_path);

        /**
         * Makes a resized copy of this Bitmap into another Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         * @param destination The Bitmap to copy to
         * @param high_quality Whether the operation should use a slower, but higher quality algorithm
         * @return Whether the operation succeeded
         */
        static jboolean resample(JNIEnv *env, jobject instance, jobject destination, bool high_quality);

        /**
         * Swaps the red and blue channels of this Bitmap.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the Bitmap
         */
        static void swap_red_blue_channels(JNIEnv *env, jobject instance);
    };
}