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