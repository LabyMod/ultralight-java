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

#include "ultralight_java/java_bridges/ultralight_platform_jni.hpp"

#include <AppCore/Platform.h>
#include <Ultralight/Ultralight.h>

#include "ultralight_java/java_bridges/bridegd_clipboard.hpp"
#include "ultralight_java/java_bridges/bridged_file_system.hpp"
#include "ultralight_java/java_bridges/bridged_gpu_driver.hpp"
#include "ultralight_java/java_bridges/bridged_logger.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    jobject UltralightPlatformJNI::global_instance = nullptr;

    jobject UltralightPlatformJNI::instance(JNIEnv *env, jclass) {
        if(!global_instance) {
            // Obtain an instance of the Ultralight platform
            auto platform_pointer = reinterpret_cast<jlong>(&ultralight::Platform::instance());

            // Create the corresponding java object
            global_instance = env->NewObject(
                runtime.ultralight_platform.clazz, runtime.ultralight_platform.constructor, platform_pointer);
        }

        return global_instance;
    }

    void UltralightPlatformJNI::set_config(JNIEnv *env, jobject java_instance, jobject java_config) {
        // Retrieve the native pointer from the java object
        auto *platform = reinterpret_cast<ultralight::Platform *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));

        // Check if an exception occurred while doing so
        if(env->ExceptionCheck()) {
            return;
        }

        auto &config_type = runtime.ultralight_config;

        // Copy over all properties from the java config
        ultralight::Config config;

        ASSIGN_CONFIG_STRING(resource_path, "resourcePath can't be null");

        auto java_cache_path = reinterpret_cast<jstring>(
            env->GetObjectField(java_config, config_type.cache_path_field));
        if(!java_cache_path) {
            // User has not set the cache path, set it to an empty string
            config.cache_path = "";
        } else {
            config.cache_path = Util::create_utf16_from_jstring(env, java_cache_path);
            if(env->ExceptionCheck()) {
                return;
            }
        }

        // Retrieve the face winding field
        jobject java_face_winding = env->GetObjectField(java_config, config_type.face_winding_field);
        if(!java_face_winding) {
            // User has set it to null
            env->ThrowNew(runtime.null_pointer_exception.clazz, "faceWinding can't be null");
            return;
        }

        if(!runtime.face_winding.constants.from_java(env, java_face_winding, &config.face_winding)) {
            // The value was invalid (shouldn't happen) and an exception has been triggered
            // by the function
            return;
        }
        // Retrieve the font hinting field
        jobject java_font_hinting = env->GetObjectField(java_config, config_type.font_hinting_field);
        if(!java_font_hinting) {
            // User has set it to null
            env->ThrowNew(runtime.null_pointer_exception.clazz, "fontHinting can't be null");
            return;
        }

        if(!runtime.font_hinting.constants.from_java(env, java_font_hinting, &config.font_hinting)) {
            // The value was invalid (shouldn't happen) and an exception has been triggered
            // by the function
            return;
        }
        ASSIGN_CONFIG(Double, font_gamma);
        ASSIGN_CONFIG_STRING(user_stylesheet, "userStylesheet can't be null");
        ASSIGN_CONFIG(Boolean, force_repaint);
        ASSIGN_CONFIG(Double, animation_timer_delay);
        ASSIGN_CONFIG(Double, scroll_timer_delay);
        ASSIGN_CONFIG(Double, recycle_delay);
        ASSIGN_CONFIG(Long, memory_cache_size);
        ASSIGN_CONFIG(Long, page_cache_size);
        ASSIGN_CONFIG(Long, override_ram_size);
        ASSIGN_CONFIG(Long, min_large_heap_size);
        ASSIGN_CONFIG(Long, min_small_heap_size);

        // And finally apply the config
        platform->set_config(config);
    }

    void UltralightPlatformJNI::use_platform_font_loader(JNIEnv *env, jobject java_instance) {
        // Retrieve the native pointer from the java object
        auto *platform = reinterpret_cast<ultralight::Platform *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));

        // Check if an exception occurred while doing so
        if(env->ExceptionCheck()) {
            return;
        }

        // Set it
        platform->set_font_loader(ultralight::GetPlatformFontLoader());
    }

    void UltralightPlatformJNI::use_platform_file_system(JNIEnv *env, jobject java_instance, jstring java_base_path) {
        // Retrieve the native pointer from the java object
        auto *platform = reinterpret_cast<ultralight::Platform *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));

        // Check if an exception occurred while doing so
        if(env->ExceptionCheck()) {
            return;
        }

        if(!java_base_path) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "basePath can't be null");
            return;
        }

        // Convert the path to an ultralight path and set the implementation
        ultralight::String16 base_path = Util::create_utf16_from_jstring(env, java_base_path);
        platform->set_file_system(ultralight::GetPlatformFileSystem(base_path));
    }

    void UltralightPlatformJNI::set_file_system(JNIEnv *env, jobject java_instance, jobject java_file_system) {
        // Retrieve the native platform pointer from the java object
        auto *platform = reinterpret_cast<ultralight::Platform *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));

        // Check if an exception occurred while doing so
        if(env->ExceptionCheck()) {
            return;
        }

        // Remove the existing file system
        platform->set_file_system(nullptr);

        // Get rid of the existing file system
        delete runtime.bridged_file_system;

        if(java_file_system) {
            // Create and set the new file system
            runtime.bridged_file_system = new BridgedFileSystem(env, java_file_system);
            platform->set_file_system(runtime.bridged_file_system);
        } else {
            // Null out the file system
            runtime.bridged_file_system = nullptr;
        }
    }

    void UltralightPlatformJNI::set_gpu_driver(JNIEnv *env, jobject java_instance, jobject java_gpu_driver) {
        // Retrieve the native platform pointer from the java object
        auto *platform = reinterpret_cast<ultralight::Platform *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));

        // Check if any exception occurred while doing so
        if (env->ExceptionCheck())
            return;

        // Remove the existing gpu driver
        platform->set_gpu_driver(nullptr);

        // Get rid of the existing gpu driver
        delete runtime.bridged_gpu_driver;

        if (java_gpu_driver) {
            // Create and set the new gpu driver
            runtime.bridged_gpu_driver = new BridgedGPUDriver(env, java_gpu_driver);
            platform->set_gpu_driver(runtime.bridged_gpu_driver);
        } else {
            // NUll out the gpu driver
            runtime.bridged_gpu_driver = nullptr;
        }

    }

    void UltralightPlatformJNI::set_gpu_driver_pointer(JNIEnv *env, jobject java_instance, jlong handle) {
        auto *platform = reinterpret_cast<ultralight::Platform *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));

        if (env->ExceptionCheck())
            return;

        platform->set_gpu_driver(nullptr);

        delete runtime.bridged_gpu_driver;
        if (handle) {
            auto *driver = (ultralight::GPUDriver *) handle;
            runtime.bridged_gpu_driver = driver;
            platform->set_gpu_driver(driver);
        } else {
            runtime.bridged_gpu_driver = nullptr;
        }
    }

    void UltralightPlatformJNI::set_clipboard(JNIEnv *env, jobject java_instance, jobject java_clipboard) {
        // Retrieve the native platform pointer from the java object
        auto *platform = reinterpret_cast<ultralight::Platform *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));

        // Check if an exception occurred while doing so
        if(env->ExceptionCheck()) {
            return;
        }

        // Remove the existing clipboard
        platform->set_clipboard(nullptr);

        // Get rid of the existing clipboard
        delete runtime.bridged_clipboard;

        if(java_clipboard) {
            // Create and set the new clipboard
            runtime.bridged_clipboard = new BridgedClipboard(env, java_clipboard);
            platform->set_clipboard(runtime.bridged_clipboard);
        } else {
            // Null out the clipboard
            runtime.bridged_clipboard = nullptr;
        }
    }

    void UltralightPlatformJNI::set_logger(JNIEnv *env, jobject java_instance, jobject java_logger) {
        // Retrieve the native platform pointer from the java object
        auto *platform = reinterpret_cast<ultralight::Platform *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));

        // Check if an exception occurred while doing so
        if(env->ExceptionCheck()) {
            return;
        }

        // Remove the existing logger
        platform->set_logger(nullptr);

        // Get rid of the existing logger
        delete runtime.bridged_logger;

        if(java_logger) {
            // Create and set the new logger
            runtime.bridged_logger = new BridgedLogger(env, java_logger);
            platform->set_logger(runtime.bridged_logger);
        } else {
            // Null out the logger
            runtime.bridged_logger = nullptr;
        }
    }

} // namespace ultralight_java