#include "ultralight_java/java_bridges/ultralight_surface_jni.hpp"

#include <Ultralight/Ultralight.h>

#include "ultralight_java/util/util.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    jlong UltralightSurfaceJNI::width(JNIEnv *env, jobject instance) {
        auto *surface = reinterpret_cast<ultralight::Surface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return 0;
        }

        return surface->width();
    }

    jlong UltralightSurfaceJNI::height(JNIEnv *env, jobject instance) {
        auto *surface = reinterpret_cast<ultralight::Surface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return 0;
        }

        return surface->height();
    }

    jlong UltralightSurfaceJNI::rowBytes(JNIEnv *env, jobject instance) {
        auto *surface = reinterpret_cast<ultralight::Surface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return 0;
        }

        return surface->row_bytes();
    }

    jlong UltralightSurfaceJNI::size(JNIEnv *env, jobject instance) {
        auto *surface = reinterpret_cast<ultralight::Surface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return 0;
        }

        return surface->size();
    }

    jobject UltralightSurfaceJNI::lockPixels(JNIEnv *env, jobject instance) {
        auto *surface = reinterpret_cast<ultralight::Surface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return nullptr;
        }

        return env->NewDirectByteBuffer(surface->LockPixels(), surface->size());
    }

    void UltralightSurfaceJNI::unlockPixels(JNIEnv *env, jobject instance) {
        auto *surface = reinterpret_cast<ultralight::Surface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return;
        }

        surface->UnlockPixels();
    }

    void UltralightSurfaceJNI::resize(JNIEnv *env, jobject instance, jlong width, jlong height) {
        auto *surface = reinterpret_cast<ultralight::Surface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return;
        }

        surface->Resize(width, height);
    }

    void UltralightSurfaceJNI::setDirtyBounds(JNIEnv *env, jobject instance, jobject rect) {
        if (!rect) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "rect can't be null");
            return;
        }

        auto *surface = reinterpret_cast<ultralight::Surface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return;
        }

        surface->set_dirty_bounds(Util::create_int_rect_from_jobject(env, rect));
    }

    jobject UltralightSurfaceJNI::dirtyBounds(JNIEnv *env, jobject instance) {
        auto *surface = reinterpret_cast<ultralight::Surface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return nullptr;
        }

        return Util::create_jobject_from_int_rect(env, surface->dirty_bounds());
    }

    void UltralightSurfaceJNI::clearDirtyBounds(JNIEnv *env, jobject instance) {
        auto *surface = reinterpret_cast<ultralight::Surface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return;
        }

        surface->ClearDirtyBounds();
    }
}