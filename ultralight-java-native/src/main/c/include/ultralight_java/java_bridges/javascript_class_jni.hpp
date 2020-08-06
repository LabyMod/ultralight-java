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