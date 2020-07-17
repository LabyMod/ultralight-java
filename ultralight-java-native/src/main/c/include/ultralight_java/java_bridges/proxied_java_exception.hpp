#pragma once

#include <jni.h>
#include <exception>

#include "ultralight_java/util/jni_reference_wrapper.hpp"

namespace ultralight_java {
    /**
     * C++ exception for a caught java exception.
     */
    class ProxiedJavaException : public std::exception {
    private:
        // The thrown object
        JNIReferenceWrapper thrown;

        /**
         * Constructs a new ProxiedJavaException with the given underlying throwable.
         *
         * @param thrown The thrown throwable
         */
        explicit ProxiedJavaException(JNIReferenceWrapper thrown);

    public:
        /**
         * Constructs a JavaProxiedException from the exception in the current JNI environment.
         * This method removes the exception from the environment. If there is no active exception,
         * then this is a no-op.
         *
         * @param env The environment to obtain the exception from
         * @throws ProxiedJavaException The currently active java exception
         */
        static void throw_if_any(JNIEnv *env);

        ProxiedJavaException(const ProxiedJavaException &other) = delete;

        /**
         * Throws this exception back to Java and clears the throwable.
         *
         * @param env The environment to throw the exception in
         */
        void throw_to_java(JNIEnv *env);
    };
}