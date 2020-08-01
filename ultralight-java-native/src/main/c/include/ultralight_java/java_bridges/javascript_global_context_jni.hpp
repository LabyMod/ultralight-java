#pragma once

#include <jni.h>

namespace ultralight_java {
    /**
     * Class for interfacing with JSGlobalContextRef from java.
     */
    class JavascriptGlobalContextJNI {
    public:
        /**
         * Releases the underlying JSContextRef instance.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this global context
         */
        static void context_unlocking(JNIEnv *env, jobject java_instance);

        /**
         * Retrieves the name of this context.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this global context
         * @return The name of this global context as a java string
         */
        static jstring get_name(JNIEnv *env, jobject java_instance);

        /**
         * Sets the name of this context.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this global context
         * @param java_name The new name of this context
         */
        static void set_name(JNIEnv *env, jobject java_instance, jstring java_name);
    };
}