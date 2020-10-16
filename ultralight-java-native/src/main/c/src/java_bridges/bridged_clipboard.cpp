/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
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

#include "ultralight_java/java_bridges/bridegd_clipboard.hpp"
#include "ultralight_java/java_bridges/proxied_java_exception.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/temporary_jni.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    BridgedClipboard::BridgedClipboard(JNIEnv *env, jobject clipboard) : JNIReferenceWrapper(env, clipboard) {
    }

    void BridgedClipboard::Clear() {
        TemporaryJNI env;

        env->CallVoidMethod(reference, runtime.ultralight_clipboard.clear_method);
        ProxiedJavaException::throw_if_any(env);
    }

    ultralight::String16 BridgedClipboard::ReadPlainText() {
        TemporaryJNI env;

        auto java_clipboard_content = reinterpret_cast<jstring>(
            env->CallObjectMethod(reference, runtime.ultralight_clipboard.read_plain_text_method));
        ProxiedJavaException::throw_if_any(env);

        if(!java_clipboard_content) {
            return ultralight::String16();
        }

        auto clipboard_content = Util::create_utf16_from_jstring(env, java_clipboard_content);
        ProxiedJavaException::throw_if_any(env);

        return clipboard_content;
    }

    void BridgedClipboard::WritePlainText(const ultralight::String16 &text) {
        TemporaryJNI env;

        auto java_text = Util::create_jstring_from_utf16(env, text);
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(reference, runtime.ultralight_clipboard.write_plain_text_method, java_text);
        ProxiedJavaException::throw_if_any(env);
    }

} // namespace ultralight_java