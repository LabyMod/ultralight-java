#pragma once

#include <jni.h>

namespace ultralight_java {
    class UltralightMatrixJNI{
    public:
        static void set(JNIEnv *env, jobject instance,
                        jdouble m11, jdouble m12, jdouble m13, jdouble m14,
                        jdouble m21, jdouble m22, jdouble m23, jdouble m24,
                        jdouble m31, jdouble m32, jdouble m33, jdouble m34,
                        jdouble m41, jdouble m42, jdouble m43, jdouble m44);
    };
}