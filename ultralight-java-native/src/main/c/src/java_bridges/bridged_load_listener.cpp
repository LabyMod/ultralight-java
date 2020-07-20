#include "ultralight_java/java_bridges/bridged_load_listener.hpp"

#include "ultralight_java/java_bridges/proxied_java_exception.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/temporary_jni.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    BridgedLoadListener::BridgedLoadListener(JNIEnv *env, jobject listener) : JNIReferenceWrapper(env, listener) {
    }

    void BridgedLoadListener::OnBeginLoading(
        ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url) {
        TemporaryJNI env;

        auto java_frame_id = static_cast<jlong>(frame_id);
        auto java_is_main_frame = static_cast<jboolean>(is_main_frame);

        auto java_url = Util::create_jstring_from_utf16(env, url.utf16());
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(
            reference,
            runtime.ultralight_load_listener.on_begin_loading_method,
            java_frame_id,
            java_is_main_frame,
            java_url);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedLoadListener::OnFinishLoading(
        ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url) {
        TemporaryJNI env;

        auto java_frame_id = static_cast<jlong>(frame_id);
        auto java_is_main_frame = static_cast<jboolean>(is_main_frame);

        auto java_url = Util::create_jstring_from_utf16(env, url.utf16());
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(
            reference,
            runtime.ultralight_load_listener.on_finish_loading_method,
            java_frame_id,
            java_is_main_frame,
            java_url);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedLoadListener::OnFailLoading(
        ultralight::View *caller,
        uint64_t frame_id,
        bool is_main_frame,
        const ultralight::String &url,
        const ultralight::String &description,
        const ultralight::String &error_domain,
        int error_code) {
        TemporaryJNI env;

        auto java_frame_id = static_cast<jlong>(frame_id);
        auto java_is_main_frame = static_cast<jboolean>(is_main_frame);

        jstring java_url = Util::create_jstring_from_utf16(env, url.utf16());
        ProxiedJavaException::throw_if_any(env);

        jstring java_description = Util::create_jstring_from_utf16(env, url.utf16());
        ProxiedJavaException::throw_if_any(env);

        jstring java_error_domain = Util::create_jstring_from_utf16(env, url.utf16());
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(
            reference,
            runtime.ultralight_load_listener.on_fail_loading_method,
            java_frame_id,
            java_is_main_frame,
            java_url,
            java_description,
            java_error_domain,
            static_cast<jint>(error_code));
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedLoadListener::OnUpdateHistory(ultralight::View *caller) {
        TemporaryJNI env;
        env->CallVoidMethod(reference, runtime.ultralight_load_listener.on_update_history_method);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedLoadListener::OnWindowObjectReady(
        ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url) {
        TemporaryJNI env;

        auto java_frame_id = static_cast<jlong>(frame_id);
        auto java_is_main_frame = static_cast<jboolean>(is_main_frame);

        jstring java_url = Util::create_jstring_from_utf16(env, url.utf16());
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(
            reference,
            runtime.ultralight_load_listener.on_window_object_ready_method,
            java_frame_id,
            java_is_main_frame,
            java_url);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedLoadListener::OnDOMReady(
        ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url) {
        TemporaryJNI env;

        auto java_frame_id = static_cast<jlong>(frame_id);
        auto java_is_main_frame = static_cast<jboolean>(is_main_frame);

        jstring java_url = Util::create_jstring_from_utf16(env, url.utf16());
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(
            reference,
            runtime.ultralight_load_listener.on_dom_ready_method,
            java_frame_id,
            java_is_main_frame,
            java_url);
        ProxiedJavaException::throw_if_any(env);
    }
} // namespace ultralight_java