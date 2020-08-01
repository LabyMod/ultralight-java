#pragma once

#include <jni.h>

namespace ultralight_java {
    /**
     * Class for interfacing with JSContextRef from java.
     */
    class JavascriptContextJNI {
    public:
        /**
         * Retrieves the global context as a java object for this context.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context
         * @return The java object for the global context of this context
         */
        static jobject get_global_context(JNIEnv *env, jobject java_instance);
    };
}