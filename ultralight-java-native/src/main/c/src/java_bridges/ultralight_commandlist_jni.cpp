//
// Created by leo on 26.08.20.
//
#include "ultralight_java/java_bridges/ultralight_commandlist_jni.hpp"

#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    jobject UltralightCommandListJNI::create(JNIEnv *env, jclass caller_class) {
        return env->NewObject(runtime.ultralight_commandlist.clazz,
                              runtime.ultralight_commandlist.constructor);
    }

    jboolean UltralightCommandListJNI::add(JNIEnv *env, jobject instance, jobject element) {
        return env->CallBooleanMethod(instance,
                                      runtime.ultralight_commandlist.add_method,
                                      element);
    }
}