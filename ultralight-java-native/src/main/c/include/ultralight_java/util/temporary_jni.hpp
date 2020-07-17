#pragma once

#include <jni.h>

namespace ultralight_java {
    /**
     * Helper for obtaining a JNI environment for a limited amount of time.
     */
    class TemporaryJNI {
    private:
        // The stored environment
        JNIEnv *env;
        bool detach_on_destruct;

    public:
        /**
         * Constructs a new TemporaryJNI environment and attaches it to the JVM
         * if required.
         */
        explicit TemporaryJNI();

        /**
         * Destructs the temporary JNI environment, detaches it from the JVM
         * if it has been attached by the constructor.
         */
        ~TemporaryJNI();

        /**
         * Retrieves the JNI environment contained by this temporary environment.
         *
         * @return The contained JNI environment
         */
        operator JNIEnv *();

        /**
         * Retrieves the JNI environment contained by this temporary environment.
         *
         * @return The contained JNI environment
         */
        JNIEnv *operator*();

        /**
          * Retrieves the JNI environment contained by this temporary environment.
          *
          * @return The contained JNI environment
          */
        JNIEnv *operator->();
    };
}