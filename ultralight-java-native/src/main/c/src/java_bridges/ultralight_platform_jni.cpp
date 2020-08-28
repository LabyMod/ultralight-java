#include "ultralight_java/java_bridges/ultralight_platform_jni.hpp"

#include <AppCore/Platform.h>
#include <Ultralight/Ultralight.h>

#include "ultralight_java/java_bridges/bridegd_clipboard.hpp"
#include "ultralight_java/java_bridges/bridged_file_system.hpp"
#include "ultralight_java/java_bridges/bridged_logger.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

#define ASSIGN_CONFIG_STRING(name, msg)                                                                                \
    if(!JNI_STRING16_OR_NPE(                                                                                           \
           config.name, env, env->GetObjectField(java_config, runtime.ultralight_config.name##_field), msg)) {         \
        return;                                                                                                        \
    }                                                                                                                  \
    void()

#define ASSIGN_CONFIG(type, name)                                                                                      \
    config.name = env->Get##type##Field(java_config, runtime.ultralight_config.name##_field);                          \
    if(env->ExceptionCheck()) {                                                                                        \
        return;                                                                                                        \
    }                                                                                                                  \
    void()

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
            env->GetObjectField(java_instance, runtime.ultralight_config.cache_path_field));
        printf("Cache path = %p\n", java_cache_path);
        if(!java_cache_path) {
            // User has not set the cache path, set it to an empty string
            config.cache_path = "";
        } else {
            config.cache_path = Util::create_utf16_from_jstring(env, java_cache_path);
            printf("Cache path2 = %p\n", java_cache_path);
            if(env->ExceptionCheck()) {
                return;
            }
        }

        ASSIGN_CONFIG(Boolean, use_gpu_renderer);
        ASSIGN_CONFIG(Double, device_scale);
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
        ASSIGN_CONFIG(Boolean, enable_images);
        ASSIGN_CONFIG(Boolean, enable_javascript);
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
        ASSIGN_CONFIG_STRING(font_family_standard, "fontFamilyStandard can't be null");
        ASSIGN_CONFIG_STRING(font_family_fixed, "fonFamilyFixed can't be null");
        ASSIGN_CONFIG_STRING(font_family_serif, "fontFamilySerif can't be null");
        ASSIGN_CONFIG_STRING(font_family_sans_serif, "fontFamilySansSerif can't be null");
        ASSIGN_CONFIG_STRING(user_agent, "userAgent can't be null");
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