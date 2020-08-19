//
// Created by leo on 26.08.20.
//
#pragma once

#include <jni.h>

namespace ultralight_java {
    class UltralightCommandListJNI {
    public:
        static jobject create(JNIEnv *env, jclass caller_class);

        /**
         * Adds an UltralightCommand to an UltralightCommandList
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java instance of the CommandList
         * @param element The Command to add to the list
         * @return See Java's ArrayList::add method
         */
        static jboolean add(JNIEnv *env, jobject instance, jobject element);
    };
}
