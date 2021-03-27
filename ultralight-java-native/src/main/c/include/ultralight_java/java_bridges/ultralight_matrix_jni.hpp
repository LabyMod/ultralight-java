#pragma once

#include <Ultralight/Ultralight.h>
#include <jni.h>

namespace ultralight_java {
    class UltralightMatrixJNI {
    public:
        static void set1(JNIEnv *env, jobject instance, jobject matrix4x4);

        static void set(
            JNIEnv *env,
            jobject instance,
            jdouble m11,
            jdouble m12,
            jdouble m13,
            jdouble m14,
            jdouble m21,
            jdouble m22,
            jdouble m23,
            jdouble m24,
            jdouble m31,
            jdouble m32,
            jdouble m33,
            jdouble m34,
            jdouble m41,
            jdouble m42,
            jdouble m43,
            jdouble m44);

        static jobject getMatrix4x4(JNIEnv *env, jobject instance);

        static jobject create(JNIEnv *env, ultralight::Matrix matrix);

        static jlong construct(JNIEnv *env, jclass caller_class);

        static void setOrthographicProjection(
            JNIEnv *env, jobject instance, jdouble width, jdouble height, jboolean flipY);

        static void transform(JNIEnv *env, jobject instance, jobject transformMatrix);

        static void _delete(JNIEnv *env, jclass caller_class, jlong handle);
    };
} // namespace ultralight_java