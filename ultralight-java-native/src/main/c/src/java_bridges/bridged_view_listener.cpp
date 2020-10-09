/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ultralight_java/java_bridges/bridged_view_listener.hpp"

#include "ultralight_java/java_bridges/proxied_java_exception.hpp"
#include "ultralight_java/java_bridges/ultralight_ref_ptr_jni.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/temporary_jni.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    BridgedViewListener::BridgedViewListener(JNIEnv *env, jobject listener) : JNIReferenceWrapper(env, listener) {
    }

    void BridgedViewListener::OnChangeTitle(ultralight::View *caller, const ultralight::String &title) {
        TemporaryJNI env;

        jstring java_title = Util::create_jstring_from_utf16(env, title.utf16());
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(reference, runtime.ultralight_view_listener.on_change_title_method, java_title);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedViewListener::OnChangeURL(ultralight::View *caller, const ultralight::String &url) {
        TemporaryJNI env;

        jstring java_url = Util::create_jstring_from_utf16(env, url.utf16());
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(reference, runtime.ultralight_view_listener.on_change_url_method, java_url);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedViewListener::OnChangeTooltip(ultralight::View *caller, const ultralight::String &tooltip) {
        TemporaryJNI env;

        jstring java_tooltip = Util::create_jstring_from_utf16(env, tooltip.utf16());
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(reference, runtime.ultralight_view_listener.on_change_tooltip_method, java_tooltip);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedViewListener::OnChangeCursor(ultralight::View *caller, ultralight::Cursor cursor) {
        TemporaryJNI env;

        jobject java_cursor = runtime.ultralight_cursor.constants.to_java(env, cursor);
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(reference, runtime.ultralight_view_listener.on_change_cursor_method, java_cursor);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedViewListener::OnAddConsoleMessage(
        ultralight::View *caller,
        ultralight::MessageSource source,
        ultralight::MessageLevel level,
        const ultralight::String &message,
        uint32_t line_number,
        uint32_t column_number,
        const ultralight::String &source_id) {
        TemporaryJNI env;

        jobject java_source = runtime.message_source.constants.to_java(env, source);
        ProxiedJavaException::throw_if_any(env);

        jobject java_level = runtime.message_level.constants.to_java(env, level);
        ProxiedJavaException::throw_if_any(env);

        jstring java_message = Util::create_jstring_from_utf16(env, message.utf16());
        ProxiedJavaException::throw_if_any(env);

        auto java_line_number = static_cast<jlong>(line_number);
        auto java_column_number = static_cast<jlong>(column_number);

        jstring java_source_id = Util::create_jstring_from_utf16(env, source_id.utf16());
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(
            reference,
            runtime.ultralight_view_listener.on_add_console_message_method,
            java_source,
            java_level,
            java_message,
            java_line_number,
            java_column_number,
            java_source_id);
        ProxiedJavaException::throw_if_any(env);
    }

    ultralight::RefPtr<ultralight::View> BridgedViewListener::OnCreateChildView(
        ultralight::View *caller,
        const ultralight::String &opener_url,
        const ultralight::String &target_url,
        bool is_popup,
        const ultralight::IntRect &popup_rect) {
        TemporaryJNI env;

        jstring java_opener_url = Util::create_jstring_from_utf16(env, opener_url.utf16());
        ProxiedJavaException::throw_if_any(env);

        jstring java_target_url = Util::create_jstring_from_utf16(env, target_url.utf16());
        ProxiedJavaException::throw_if_any(env);

        auto java_is_popup = static_cast<jboolean>(is_popup);

        jobject java_popup_rect = Util::create_jobject_from_int_rect(env, popup_rect);
        ProxiedJavaException::throw_if_any(env);

        jobject java_new_view = env->CallObjectMethod(
            reference,
            runtime.ultralight_view_listener.on_create_child_view_method,
            java_opener_url,
            java_target_url,
            java_is_popup,
            java_popup_rect);

        if(!java_new_view) {
            return nullptr;
        }

        return UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::View>(env, java_new_view);
    }
} // namespace ultralight_java