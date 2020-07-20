#pragma once

#include <jni.h>

namespace ultralight_java {
    /**
     * Class used for interfacing with ultralight::Platform from java.
     */
    class UltralightPlatformJNI {
    private:
        // Global ref to a java instance object
        static jobject global_instance;

    public:
        /**
         * Runs the runnable given on thread which is safe to use ultralight on.
         * The JVM seems to set up the stack in an unusual way, which then causes the
         * JavaScript engine from WebCore to fail.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The java class calling the method, should always be UltralightPlatform
         * @param runnable The runnable to run on the safe thread
         */
        static void run_on_safe_thread(JNIEnv *env, jclass caller_class, jobject runnable);

        /**
         * Retrieves the java object matching the platform instance object.
         * If the instance does not exist yet, it is created.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The java class calling the method, should always be UltralightPlatform
         * @return The java platform instance object
         */
        static jobject instance(JNIEnv *env, jclass caller_class);

        /**
         * Sets the platform configuration object from the java object.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The instance of the platform as a java object
         * @param java_config The new configuration as a java object
         */
        static void set_config(JNIEnv *env, jobject java_instance, jobject java_config);

        /**
         * Sets the font loader to the platform dependent implementation.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The instance of the platform as a java object
         */
        static void use_platform_font_loader(JNIEnv *env, jobject java_instance);

        /**
         * Sets the file system to the platform dependent implementation.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The instance of the platform as a java object
         * @param java_base_path The base path to resolve file:/// URL's relative to
         */
        static void use_platform_file_system(JNIEnv *env, jobject java_instance, jstring java_base_path);

        /**
         * Sets the internal logger to the provided java one.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The instance of the platform as a java object
         * @param java_logger
         */
        static void set_logger(JNIEnv *env, jobject java_instance, jobject java_logger);
    };
}