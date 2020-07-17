#include "ultralight_java/util/jni_reference_wrapper.hpp"

#include "ultralight_java/util/temporary_jni.hpp"

namespace ultralight_java {
    JNIReferenceWrapper::JNIReferenceWrapper(JNIReferenceWrapper &&other) noexcept: reference(other.reference) {
        other.reference = nullptr;
    }

    JNIReferenceWrapper::JNIReferenceWrapper(JNIEnv *env, jobject reference) : reference(
            env->NewGlobalRef(reference)) {}

    JNIReferenceWrapper::~JNIReferenceWrapper() {
        if (reference) {
            TemporaryJNI jni;
            jni->DeleteGlobalRef(reference);
        }
    }

    JNIReferenceWrapper::operator jobject() {
        return reference;
    }

    jobject JNIReferenceWrapper::operator*() {
        return reference;
    }

    jobject JNIReferenceWrapper::get() {
        return reference;
    }
}