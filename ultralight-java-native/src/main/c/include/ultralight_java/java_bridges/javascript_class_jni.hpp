#pragma once

#include <jni.h>

namespace ultralight_java {
    /**
     * Class for interfacing with JSClassRef from java
     */
    class JavascriptClassJNI {
    public:
        /**
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The java class calling the method, should always be JavascriptClass
         * @param handle The handle of the JSClassRef to release
         */
        static void release(JNIEnv *env, jclass caller_class, jlong handle);
    };
}