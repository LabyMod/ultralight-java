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

#include "ultralight_java/java_bridges/bridged_file_system.hpp"

#include "ultralight_java/java_bridges/proxied_java_exception.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/temporary_jni.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    BridgedFileSystem::BridgedFileSystem(JNIEnv *env, jobject file_system) : JNIReferenceWrapper(env, file_system) {
    }

    bool BridgedFileSystem::FileExists(const ultralight::String16 &path) {
        TemporaryJNI env;

        jstring java_path = Util::create_jstring_from_utf16(env, path);
        ProxiedJavaException::throw_if_any(env);

        jboolean
            result = env->CallBooleanMethod(reference, runtime.ultralight_file_system.file_exists_method, java_path);
        ProxiedJavaException::throw_if_any(env);

        return result;
    }

    bool BridgedFileSystem::GetFileSize(ultralight::FileHandle handle, int64_t &result) {
        TemporaryJNI env;

        auto java_handle = static_cast<jlong>(handle);
        jlong java_result =
            env->CallLongMethod(reference, runtime.ultralight_file_system.get_file_size_method, java_handle);

        if(java_result < 0) {
            return false;
        } else {
            result = java_result;
            return true;
        }
    }

    bool BridgedFileSystem::GetFileMimeType(const ultralight::String16 &path, ultralight::String16 &result) {
        TemporaryJNI env;

        jstring java_path = Util::create_jstring_from_utf16(env, path);
        ProxiedJavaException::throw_if_any(env);

        auto java_result = reinterpret_cast<jstring>(
            env->CallObjectMethod(reference, runtime.ultralight_file_system.get_file_mime_type_method, java_path));

        if(!java_result) {
            return false;
        } else {
            result = Util::create_utf16_from_jstring(env, java_result);
            ProxiedJavaException::throw_if_any(env);
            return true;
        }
    }

    ultralight::FileHandle BridgedFileSystem::OpenFile(const ultralight::String16 &path, bool open_for_writing) {
        TemporaryJNI env;

        jstring java_path = Util::create_jstring_from_utf16(env, path);
        ProxiedJavaException::throw_if_any(env);

        auto java_open_for_writing = static_cast<jboolean>(open_for_writing);

        jlong java_result = env->CallLongMethod(
            reference, runtime.ultralight_file_system.open_file_method, java_path, java_open_for_writing);
        ProxiedJavaException::throw_if_any(env);

        return static_cast<ultralight::FileHandle>(java_result);
    }

    void BridgedFileSystem::CloseFile(ultralight::FileHandle &handle) {
        TemporaryJNI env;

        auto java_handle = static_cast<jlong>(handle);

        env->CallVoidMethod(reference, runtime.ultralight_file_system.close_file_method, java_handle);
        ProxiedJavaException::throw_if_any(env);
    }

    int64_t BridgedFileSystem::ReadFromFile(ultralight::FileHandle handle, char *data, int64_t length) {
        TemporaryJNI env;

        auto java_handle = static_cast<jlong>(handle);
        auto java_data = env->NewDirectByteBuffer(data, length);
        auto java_length = static_cast<jlong>(length);

        jlong java_result = env->CallLongMethod(
            reference, runtime.ultralight_file_system.read_from_file_method, java_handle, java_data, java_length);
        ProxiedJavaException::throw_if_any(env);

        return static_cast<int64_t>(java_result);
    }
} // namespace ultralight_java