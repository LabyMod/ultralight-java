#include "ultralight_java/java_bridges/javascript_context_jni.hpp"

#include <JavaScriptCore/JavaScript.h>

#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    jobject JavascriptContextJNI::get_global_context(JNIEnv *env, jobject java_instance) {
        auto context = reinterpret_cast<JSContextRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        jobject lock = env->CallObjectMethod(java_instance, runtime.javascript_locked_object.get_lock_method);
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        auto global_context = JSGlobalContextRetain(JSContextGetGlobalContext(context));

        return env->NewObject(
            runtime.javascript_global_context.clazz,
            runtime.javascript_global_context.constructor,
            reinterpret_cast<jlong>(global_context),
            lock);
    }
} // namespace ultralight_java