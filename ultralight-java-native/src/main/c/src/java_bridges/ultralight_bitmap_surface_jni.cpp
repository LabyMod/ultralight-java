#include "ultralight_java/java_bridges/ultralight_bitmap_surface_jni.hpp"

#include "ultralight_java/java_bridges/ultralight_ref_ptr_jni.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    jobject UltralightBitmapSurfaceJNI::bitmap(JNIEnv *env, jobject instance) {
        auto *surface = reinterpret_cast<ultralight::BitmapSurface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return nullptr;
        }

        auto bitmap = surface->bitmap();
        return env->NewObject(runtime.ultralight_bitmap.clazz, runtime.ultralight_bitmap.constructor,
                              UltralightRefPtrJNI::create<ultralight::Bitmap>(env, std::move(
                                      ultralight::RefPtr<ultralight::Bitmap>(std::move(bitmap)))));
    }
}