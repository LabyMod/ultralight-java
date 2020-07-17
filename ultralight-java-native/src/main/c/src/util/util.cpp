#include "ultralight_java/util/util.hpp"

#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
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
                int_rect.bottom
        );
    }

    ultralight::BitmapFormat Util::create_bitmap_format_from_jobject(JNIEnv *env, jobject format) {
        if (env->IsSameObject(
                format,
                env->GetStaticObjectField(runtime.ultralight_bitmap_format.clazz,
                                          runtime.ultralight_bitmap_format.a8_unorm_field)
        )) {
            return ultralight::kBitmapFormat_A8_UNORM;
        } else if (env->IsSameObject(
                format,
                env->GetStaticObjectField(runtime.ultralight_bitmap_format.clazz,
                                          runtime.ultralight_bitmap_format.bgra8_unorm_srgb_field)
        )) {
            return ultralight::kBitmapFormat_BGRA8_UNORM_SRGB;
        } else {
            env->ThrowNew(runtime.illegal_state_exception.clazz, "Invalid UltralightBitmapFormat passed");
            return ultralight::kBitmapFormat_BGRA8_UNORM_SRGB;
        }
    }

    jobject Util::create_jobject_from_bitmap_format(JNIEnv *env, const ultralight::BitmapFormat &format) {
        switch (format) {
            case ultralight::kBitmapFormat_A8_UNORM:
                return env->GetStaticObjectField(runtime.ultralight_bitmap_format.clazz,
                                                 runtime.ultralight_bitmap_format.a8_unorm_field);

            case ultralight::kBitmapFormat_BGRA8_UNORM_SRGB:
                return env->GetStaticObjectField(runtime.ultralight_bitmap_format.clazz,
                                                 runtime.ultralight_bitmap_format.bgra8_unorm_srgb_field);

            default:
                env->ThrowNew(runtime.illegal_state_exception.clazz, "Native sent invalid enum constant");
                return nullptr;
        }
    }
}