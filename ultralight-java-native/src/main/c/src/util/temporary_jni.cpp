#include "ultralight_java/util/temporary_jni.hpp"

#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    TemporaryJNI::TemporaryJNI() {
        // Try to get a JNI environment
        int error = runtime.vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_8);
        if(error == JNI_EDETACHED) {
            // The current thread is detached, attach it and obtain the JNI environment
            // using the attach function
            runtime.vm->AttachCurrentThread(reinterpret_cast<void **>(&env), nullptr);
            detach_on_destruct = true;
        } else {
            detach_on_destruct = false;
        }
    }

    TemporaryJNI::~TemporaryJNI() {
        if(detach_on_destruct) {
            // The detach flag is set, detach the thread now
            runtime.vm->DetachCurrentThread();
        }
    }

    TemporaryJNI::operator JNIEnv *() {
        return env;
    }

    JNIEnv *TemporaryJNI::operator*() {
        return env;
    }

    JNIEnv *TemporaryJNI::operator->() {
        return env;
    }
}