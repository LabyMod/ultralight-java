#include "ultralight_java/java_bridges/proxied_java_exception.hpp"

namespace ultralight_java {
    ProxiedJavaException::ProxiedJavaException(JNIReferenceWrapper thrown) : thrown(std::move(thrown)) {}

    void ProxiedJavaException::throw_if_any(JNIEnv *env) {
        if(!env->ExceptionCheck()) {
            // Can't throw anything
            return;
        } else {
            // Retrieve the current exception and remove it from the environment
            jthrowable thrown = env->ExceptionOccurred();
            env->ExceptionClear();

            // Create a proxied exception and throw it
            throw ProxiedJavaException(JNIReferenceWrapper(env, thrown));
        }
    }

    void ProxiedJavaException::throw_to_java(JNIEnv *env) {
        env->Throw(reinterpret_cast<jthrowable>(*thrown));
    }
}