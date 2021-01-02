/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2021 LabyMedia and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "ultralight_java/util/util.hpp"

#include <ultralight_java/ultralight_java_instance.hpp>

#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    template <typename T>
    constexpr const jchar *to_jchar_ptr(const T *value) {
        return reinterpret_cast<const jchar *>(value);
    }

    template <>
    constexpr const jchar *to_jchar_ptr(const jchar *value) {
        return value;
    }

    template <typename T>
    constexpr const JSChar *to_jschar_ptr(const T *value) {
        return reinterpret_cast<const JSChar *>(value);
    }

    template <>
    constexpr const JSChar *to_jschar_ptr(const JSChar *value) {
        return value;
    }

    std::string Util::create_utf8_from_jsstring_ref(JSStringRef str) {
        // Get the maximum conversion length
        size_t buffer_size = JSStringGetMaximumUTF8CStringSize(str);

        if(buffer_size < 1) {
            // Check for 0 length
            return "";
        }

        // Allocate a buffer to store the data in
        char *buffer = new char[buffer_size];

        // Convert the string
        size_t real_size = JSStringGetUTF8CString(str, buffer, buffer_size);

        // Copy the buffer length into a std::string
        // This constructor takes the length without the null byte
        std::string ret(buffer, real_size - 1);

        // Clean up and return
        delete[] buffer;
        return ret;
    }

    ultralight::String16 Util::create_utf16_from_jstring(JNIEnv *env, jstring str) {
        // Acquire the UTF16 chars of the String and its length
        const jchar *chars = env->GetStringChars(str, nullptr);
        size_t char_count = env->GetStringLength(str);

        // Copy the string
        ultralight::String16 converted(chars, char_count);

        // Release the string chars to prevent memory leaks
        env->ReleaseStringChars(str, chars);

        return converted;
    }

    jstring Util::create_jstring_from_utf16(JNIEnv *env, const ultralight::String16 &str) {
        // Acquire the UTF16 chars of the String and its length
        const jchar *chars = str.udata();
        size_t char_count = str.length();

        // Create the string
        return env->NewString(chars, char_count);
    }

    ultralight::IntRect Util::create_int_rect_from_jobject(JNIEnv *env, jobject object) {
        // Extract the single fields
        int left = env->GetIntField(object, runtime.int_rect.left_field);
        int top = env->GetIntField(object, runtime.int_rect.top_field);
        int right = env->GetIntField(object, runtime.int_rect.right_field);
        int bottom = env->GetIntField(object, runtime.int_rect.bottom_field);

        // Create the IntRect
        return ultralight::IntRect{left, top, right, bottom};
    }

    jobject Util::create_jobject_from_int_rect(JNIEnv *env, const ultralight::IntRect &int_rect) {
        return env->NewObject(
            runtime.int_rect.clazz,
            runtime.int_rect.bounds_constructor,
            int_rect.left,
            int_rect.top,
            int_rect.right,
            int_rect.bottom);
    }

    ultralight::KeyEvent Util::create_key_event_from_jobject(JNIEnv *env, jobject event) {
        const auto &t = runtime.ultralight_key_event;

        auto java_event_type = env->GetObjectField(event, t.type_field);
        if(!java_event_type) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "eventType can't be null");
            return ultralight::KeyEvent{};
        }

        ultralight::KeyEvent::Type event_type;
        if(!runtime.ultralight_key_event_type.constants.from_java(env, java_event_type, &event_type)) {
            return ultralight::KeyEvent{};
        }

        unsigned modifiers = env->GetIntField(event, t.modifiers_field);

        auto java_virtual_key_code = env->GetObjectField(event, t.virtual_key_code_field);
        int virtual_key_code;
        if(!java_virtual_key_code) {
            virtual_key_code = ultralight::KeyCodes::GK_UNKNOWN;
        } else {
            virtual_key_code = env->GetIntField(java_virtual_key_code, runtime.ultralight_key.id_field);
        }

        int native_key_code = env->GetIntField(event, t.native_key_code_field);

        auto java_key_identifier = env->GetObjectField(event, t.key_identifier_field);
        ultralight::String key_identifier;
        if(!java_key_identifier) {
            key_identifier = "";
        } else {
            key_identifier = create_utf16_from_jstring(env, reinterpret_cast<jstring>(java_key_identifier));
        }

        auto java_text = env->GetObjectField(event, t.text_field);
        ultralight::String text;
        if(!java_text) {
            text = "";
        } else {
            text = create_utf16_from_jstring(env, reinterpret_cast<jstring>(java_text));
        }

        auto java_unmodified_text = env->GetObjectField(event, t.unmodified_text_field);
        ultralight::String unmodified_text;
        if(!java_unmodified_text) {
            unmodified_text = "";
        } else {
            unmodified_text = create_utf16_from_jstring(env, reinterpret_cast<jstring>(java_unmodified_text));
        }

        bool is_keypad = env->GetBooleanField(event, t.is_keypad_field);
        bool is_auto_repeat = env->GetBooleanField(event, t.is_auto_repeat_field);
        bool is_system_key = env->GetBooleanField(event, t.is_system_key_field);

        ultralight::KeyEvent out;
        out.type = event_type;
        out.modifiers = modifiers;
        out.virtual_key_code = virtual_key_code;
        out.native_key_code = native_key_code;
        out.key_identifier = key_identifier;
        out.text = text;
        out.unmodified_text = unmodified_text;
        out.is_keypad = is_keypad;
        out.is_auto_repeat = is_auto_repeat;
        out.is_system_key = is_system_key;

        return std::move(out);
    }

    ultralight::MouseEvent Util::create_mouse_event_from_jobject(JNIEnv *env, jobject event) {
        const auto &t = runtime.ultralight_mouse_event;

        auto java_type = env->GetObjectField(event, t.type_field);
        if(!java_type) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "type can't be null");
            return ultralight::MouseEvent{};
        }

        ultralight::MouseEvent::Type type;
        if(!runtime.ultralight_mouse_event_type.constants.from_java(env, java_type, &type)) {
            return ultralight::MouseEvent{};
        }

        int x = env->GetIntField(event, runtime.ultralight_mouse_event.x_field);
        int y = env->GetIntField(event, runtime.ultralight_mouse_event.y_field);

        auto java_button = env->GetObjectField(event, runtime.ultralight_mouse_event.button_field);

        ultralight::MouseEvent::Button button;
        if(!java_button) {
            button = ultralight::MouseEvent::Button::kButton_None;
        } else {
            if(!runtime.ultralight_mouse_event_button.constants.from_java(env, java_button, &button)) {
                return ultralight::MouseEvent{};
            }
        }

        return ultralight::MouseEvent{type, x, y, button};
    }

    ultralight::ScrollEvent Util::create_scroll_event_from_jobject(JNIEnv *env, jobject event) {
        const auto &t = runtime.ultralight_scroll_event;

        auto java_type = env->GetObjectField(event, t.type_field);
        if(!java_type) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "type can't be null");
            return ultralight::ScrollEvent{};
        }

        ultralight::ScrollEvent::Type type;
        if(!runtime.ultralight_scroll_event_type.constants.from_java(env, java_type, &type)) {
            return ultralight::ScrollEvent{};
        }

        int delta_x = env->GetIntField(event, t.delta_x_field);
        int delta_y = env->GetIntField(event, t.delta_y_field);

        return ultralight::ScrollEvent{type, delta_x, delta_y};
    }

    jstring Util::create_jstring_from_jsstring_ref(JNIEnv *env, JSStringRef javascript_string) {
        const JSChar *javascript_chars = JSStringGetCharactersPtr(javascript_string);
        size_t javascript_chars_length = JSStringGetLength(javascript_string);

        return env->NewString(to_jchar_ptr(javascript_chars), javascript_chars_length);
    }

    JSStringRef Util::create_jsstring_ref_from_jstring(JNIEnv *env, jstring java_string) {
        const jchar *java_chars = env->GetStringChars(java_string, nullptr);
        size_t java_chars_length = env->GetStringLength(java_string);

        auto javascript_string = JSStringCreateWithCharacters(to_jschar_ptr(java_chars), java_chars_length);
        env->ReleaseStringChars(java_string, java_chars);

        return javascript_string;
    }

    void Util::throw_jssvalue_ref_as_java_exception(
        const std::string &message, JSContextRef context, JSValueRef javascript_value, JNIEnv *env, jobject lock) {
        JSValueProtect(context, javascript_value);

        jobject value = env->NewObject(
            runtime.javascript_value.clazz,
            runtime.javascript_value.constructor,
            reinterpret_cast<JSValueRef>(javascript_value),
            lock);
        if(env->ExceptionCheck()) {
            JSValueUnprotect(context, javascript_value);
            return;
        }

        jstring exception_message = env->NewStringUTF(message.c_str());
        auto exception = reinterpret_cast<jthrowable>(env->NewObject(
            runtime.javascript_exception.clazz, runtime.javascript_exception.constructor, exception_message, value));
        env->DeleteLocalRef(exception_message);
        env->Throw(exception);
    }

    JSValueRef Util::create_jssvalue_from_jthrowable(JNIEnv *env, jthrowable java_throwable, JSContextRef context) {
        auto java_exception_message = reinterpret_cast<jstring>(
            env->CallObjectMethod(java_throwable, runtime.throwable.get_message_method));
        JSStringRef javascript_exception_message = ([&] {
            if(env->IsInstanceOf(java_throwable, runtime.null_pointer_exception.clazz) && !java_exception_message) {
                return JSStringCreateWithUTF8CString("Nullpointer exception");
            }

            return java_exception_message ? Util::create_jsstring_ref_from_jstring(env, java_exception_message) :
                                            JSStringCreateWithUTF8CString("<No error message provided>");
        })();

        if(java_exception_message) {
            env->DeleteLocalRef(java_exception_message);
        }

        JSValueRef arguments[] = {JSValueMakeString(context, javascript_exception_message)};

        JSValueRef javascript_exception_exception = nullptr;
        JSObjectRef javascript_exception = JSObjectMakeError(context, 1, arguments, &javascript_exception_exception);

        JSValueUnprotect(context, arguments[0]);

        if(javascript_exception_exception) {
            return javascript_exception_exception;
        }

        JSStringRelease(javascript_exception_message);
        return javascript_exception;
    }

    JSValueRef Util::create_jserror(JSContextRef context, const std::string &message) {
        JSStringRef javascript_exception_message = JSStringCreateWithUTF8CString(message.c_str());
        JSValueRef arguments[] = {JSValueMakeString(context, javascript_exception_message)};

        JSValueRef javascript_exception_exception = nullptr;
        JSObjectRef javascript_exception = JSObjectMakeError(context, 1, arguments, &javascript_exception_exception);

        JSValueUnprotect(context, arguments[0]);

        if(javascript_exception_exception) {
            return javascript_exception_exception;
        }

        JSStringRelease(javascript_exception_message);
        return javascript_exception;
    }

    JSValueRef *Util::translate_bridged_arguments(JNIEnv *env, jobjectArray java_arguments) {
        size_t length = env->GetArrayLength(java_arguments);
        if(length == 0) {
            return nullptr;
        }

        auto *values = new JSValueRef[length];
        for(size_t i = 0; i < length; i++) {
            jobject java_argument = env->GetObjectArrayElement(java_arguments, i);
            values[i] = reinterpret_cast<JSValueRef>(
                env->CallLongMethod(java_argument, runtime.object_with_handle.get_handle_method));
            env->DeleteLocalRef(java_argument);

            if(env->ExceptionCheck()) {
                delete[] values;
                return nullptr;
            }
        }

        return values;
    }
} // namespace ultralight_java