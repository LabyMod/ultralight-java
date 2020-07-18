#include "ultralight_java/java_bridges/ultralight_bitmap_jni.hpp"

#include "ultralight_java/java_bridges/ultralight_ref_ptr_jni.hpp"
#include "ultralight_java/util/util.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    jobject UltralightBitmapJNI::create(JNIEnv *env, jclass) {
        auto ptr = UltralightRefPtrJNI::create<ultralight::Bitmap>(env,
                                                                   std::move(ultralight::RefPtr<ultralight::Bitmap>(
                                                                           std::move(ultralight::Bitmap::Create()))));

        return env->NewObject(runtime.ultralight_bitmap.clazz, runtime.ultralight_bitmap.constructor, ptr);
    }

    jobject UltralightBitmapJNI::create(JNIEnv *env, jclass, jlong width, jlong height, jobject format) {
        if (!format) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "format can't be null");
            return nullptr;
        }

        ultralight::BitmapFormat real_format;
        if(!runtime.ultralight_bitmap_format.constants.from_java(env, format, &real_format)) {
            return nullptr;
        }

        auto ptr = UltralightRefPtrJNI::create<ultralight::Bitmap>(env, std::move(
                ultralight::RefPtr<ultralight::Bitmap>(
                        std::move(ultralight::Bitmap::Create(width, height, real_format)))));

        return env->NewObject(runtime.ultralight_bitmap.clazz, runtime.ultralight_bitmap.constructor, ptr);
    }

    jobject UltralightBitmapJNI::create(JNIEnv *env, jclass, jlong width, jlong height, jobject format,
                                        jlong row_bytes, jobject pixel_buffer, jboolean fixup_gamma) {
        if (!format) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "format can't be null");
            return nullptr;
        } else if (!pixel_buffer) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "pixels can't be null");
            return nullptr;
        }

        ultralight::BitmapFormat real_format;
        if(!runtime.ultralight_bitmap_format.constants.from_java(env, format, &real_format)) {
            return nullptr;
        }

        void *data = env->GetDirectBufferAddress(pixel_buffer);
        if (!data) {
            env->ThrowNew(runtime.illegal_argument_exception.clazz, "The pixel buffer needs to be a direct one");
            return nullptr;
        }

        size_t data_size = env->GetDirectBufferCapacity(pixel_buffer);

        auto ptr = UltralightRefPtrJNI::create<ultralight::Bitmap>(env, std::move(
                ultralight::RefPtr<ultralight::Bitmap>(
                        std::move(
                                ultralight::Bitmap::Create(width, height, real_format, row_bytes, data, data_size, true,
                                                           fixup_gamma)))));

        return env->NewObject(runtime.ultralight_bitmap.clazz, runtime.ultralight_bitmap.constructor, ptr);
    }

    jobject UltralightBitmapJNI::create(JNIEnv *env, jclass, jobject bitmap) {
        if (!bitmap) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "bitmap can't be null");
            return nullptr;
        }

        auto real_bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, bitmap);
        auto ptr = UltralightRefPtrJNI::create<ultralight::Bitmap>(env, std::move(
                ultralight::RefPtr<ultralight::Bitmap>(ultralight::Bitmap::Create(*real_bitmap))));

        return env->NewObject(runtime.ultralight_bitmap.clazz, runtime.ultralight_bitmap.constructor, ptr);
    }

    jlong UltralightBitmapJNI::width(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return 0;
        }

        return bitmap->width();
    }

    jlong UltralightBitmapJNI::height(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return 0;
        }

        return bitmap->height();
    }

    jobject UltralightBitmapJNI::bounds(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return nullptr;
        }

        return Util::create_jobject_from_int_rect(env, bitmap->bounds());
    }

    jobject UltralightBitmapJNI::format(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return nullptr;
        }

        return runtime.ultralight_bitmap_format.constants.to_java(env, bitmap->format());
    }

    jlong UltralightBitmapJNI::bpp(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return 0;
        }

        return bitmap->bpp();
    }

    jlong UltralightBitmapJNI::row_bytes(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return 0;
        }

        return bitmap->row_bytes();
    }

    jlong UltralightBitmapJNI::size(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return 0;
        }

        return bitmap->size();
    }

    jboolean UltralightBitmapJNI::owns_pixels(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return 0;
        }

        return bitmap->owns_pixels();
    }

    jobject UltralightBitmapJNI::lock_pixels(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return nullptr;
        }

        return env->NewDirectByteBuffer(bitmap->LockPixels(), bitmap->size());
    }

    void UltralightBitmapJNI::unlock_pixels(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return;
        }

        bitmap->UnlockPixels();
    }

    jobject UltralightBitmapJNI::raw_pixels(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return nullptr;
        }

        return env->NewDirectByteBuffer(bitmap->raw_pixels(), bitmap->size());
    }

    jboolean UltralightBitmapJNI::is_emtpy(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return false;
        }

        return bitmap->IsEmpty();
    }

    void UltralightBitmapJNI::erase(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return;
        }

        bitmap->Erase();
    }

    void UltralightBitmapJNI::set(JNIEnv *env, jobject instance, jobject other) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return;
        }

        if (!other) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "bitmap can't be null");
            return;
        }

        auto other_bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, other);
        if (env->ExceptionCheck()) {
            return;
        }

        bitmap->Set(*other_bitmap);
    }

    jboolean
    UltralightBitmapJNI::draw_bitmap(JNIEnv *env, jobject instance, jobject java_src_rect, jobject java_dest_rect,
                                     jobject java_src, bool pad_repeat) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return false;
        }

        if (!java_src_rect) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "srcRect can't be null");
            return false;
        } else if (!java_dest_rect) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "destRect can't be null");
            return false;
        } else if (!java_src) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "src can't be null");
            return false;
        }

        auto src_rect = Util::create_int_rect_from_jobject(env, java_src_rect);
        if (env->ExceptionCheck()) {
            return false;
        }

        auto dest_rect = Util::create_int_rect_from_jobject(env, java_dest_rect);
        if (env->ExceptionCheck()) {
            return false;
        }

        auto src_bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, java_src);
        if (env->ExceptionCheck()) {
            return false;
        }

        return bitmap->DrawBitmap(src_rect, dest_rect, *src_bitmap, pad_repeat);
    }

    jboolean UltralightBitmapJNI::write_png(JNIEnv *env, jobject instance, jstring java_path) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return false;
        }

        if (!java_path) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "path can't be null");
            return false;
        }

        const char *path = env->GetStringUTFChars(java_path, nullptr);
        bool result = bitmap->WritePNG(path);
        env->ReleaseStringUTFChars(java_path, path);

        return result;
    }

    jboolean UltralightBitmapJNI::resample(JNIEnv *env, jobject instance, jobject destination, bool high_quality) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return false;
        }

        if (!destination) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "destination can't be null");
            return false;
        }

        auto destination_bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, destination);
        if (env->ExceptionCheck()) {
            return false;
        }

        return bitmap->Resample(*destination_bitmap, high_quality);
    }

    void UltralightBitmapJNI::swap_red_blue_channels(JNIEnv *env, jobject instance) {
        auto bitmap = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Bitmap>(env, instance);
        if (env->ExceptionCheck()) {
            return;
        }

        bitmap->SwapRedBlueChannels();
    }
}