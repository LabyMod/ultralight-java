#include "ultralight_java/java_bridges/javascript_global_context_jni.hpp"

#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    void JavascriptGlobalContextJNI::context_unlocking(JNIEnv *env, jobject java_instance) {
        auto context = reinterpret_cast<JSGlobalContextRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return;
        }

        JSGlobalContextRelease(context);
    }

    jstring JavascriptGlobalContextJNI::get_name(JNIEnv *env, jobject java_instance) {
        auto context = reinterpret_cast<JSGlobalContextRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        auto javascript_name = JSGlobalContextCopyName(context);
        if(!javascript_name) {
            return nullptr;
        }

        auto java_name = Util::create_jstring_from_jsstring_ref(env, javascript_name);
        JSStringRelease(javascript_name);

        return java_name;
    }

    void JavascriptGlobalContextJNI::set_name(JNIEnv *env, jobject java_instance, jstring java_name) {
        auto context = reinterpret_cast<JSGlobalContextRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return;
        }

        if(!java_name) {
            JSGlobalContextSetName(context, nullptr);
            return;
        }

        auto javascript_name = Util::create_jsstring_ref_from_jstring(env, java_name);
        JSGlobalContextSetName(context, javascript_name);
    }
} // namespace ultralight_java