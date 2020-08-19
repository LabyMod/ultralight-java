//
// Created by leo on 18.08.20.
//
#include <ultralight_java/java_bridges/bridged_gpu_driver.hpp>

#include "ultralight_java/java_bridges/proxied_java_exception.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/temporary_jni.hpp"
#include "ultralight_java/util/util.hpp"
#include "ultralight_java/java_bridges/ultralight_ref_ptr_jni.hpp"

namespace ultralight_java {

    BridgedGPUDriver::BridgedGPUDriver(JNIEnv *env, jobject gpu_driver) : JNIReferenceWrapper(env, gpu_driver) {
    }

    void BridgedGPUDriver::BeginSynchronize() {
        TemporaryJNI env;

        env->CallVoidMethod(reference, runtime.ultralight_gpu_driver.begin_synchronize_method);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedGPUDriver::EndSynchronize() {
        TemporaryJNI env;

        env->CallVoidMethod(reference, runtime.ultralight_gpu_driver.end_synchronize_method);
        ProxiedJavaException::throw_if_any(env);
    }

    uint32_t BridgedGPUDriver::NextTextureId() {
        TemporaryJNI env;

        jlong java_result = env->CallLongMethod(reference,
                                                runtime.ultralight_gpu_driver.next_texture_id_method);
        ProxiedJavaException::throw_if_any(env);

        return static_cast<uint32_t>(java_result);
    }

    void BridgedGPUDriver::CreateTexture(uint32_t texture_id, ultralight::Ref<ultralight::Bitmap> bitmap) {
        TemporaryJNI env;

        auto jni_bitmap = env->NewObject(runtime.ultralight_bitmap.clazz,
                                         runtime.ultralight_bitmap.constructor,
                                         UltralightRefPtrJNI::create<ultralight::Bitmap>(
                                             env,
                                             std::move(
                                                 ultralight::RefPtr<ultralight::Bitmap>(
                                                    std::move(bitmap)))));
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(reference,
                            runtime.ultralight_gpu_driver.create_texture_method,
                            static_cast<jlong>(texture_id),
                            jni_bitmap);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedGPUDriver::UpdateTexture(uint32_t texture_id, ultralight::Ref<ultralight::Bitmap> bitmap) {
        TemporaryJNI env;

        auto jni_bitmap = env->NewObject(runtime.ultralight_bitmap.clazz,
                                         runtime.ultralight_bitmap.constructor,
                                         UltralightRefPtrJNI::create<ultralight::Bitmap>(
                                             env,
                                             std::move(
                                                 ultralight::RefPtr<ultralight::Bitmap>(
                                                     std::move(bitmap)))));
        ProxiedJavaException::throw_if_any(env);

        env->CallVoidMethod(reference,
                            runtime.ultralight_gpu_driver.update_texture_method,
                            static_cast<jlong>(texture_id),
                            jni_bitmap);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedGPUDriver::DestroyTexture(uint32_t texture_id) {
        TemporaryJNI env;

        env->CallVoidMethod(reference,
                            runtime.ultralight_gpu_driver.destroy_texture_method,
                            static_cast<jlong>(texture_id));
        ProxiedJavaException::throw_if_any(env);
    }

    uint32_t BridgedGPUDriver::NextRenderBufferId() {
        TemporaryJNI env;

        jlong result = env->CallLongMethod(reference,
                                           runtime.ultralight_gpu_driver.next_render_buffer_id_method);
        ProxiedJavaException::throw_if_any(env);

        return static_cast<uint32_t>(result);
    }

    void BridgedGPUDriver::CreateRenderBuffer(uint32_t render_buffer_id,
                                              const ultralight::RenderBuffer &buffer) {
        TemporaryJNI env;
        // TODO
    }

    void BridgedGPUDriver::DestroyRenderBuffer(uint32_t render_buffer_id) {
        TemporaryJNI env;

        env->CallVoidMethod(reference,
                            runtime.ultralight_gpu_driver.destroy_render_buffer_method,
                            static_cast<jlong>(render_buffer_id));
        ProxiedJavaException::throw_if_any(env);
    }

    uint32_t BridgedGPUDriver::NextGeometryId() {
        TemporaryJNI env;

        jlong result = env->CallLongMethod(reference,
                                           runtime.ultralight_gpu_driver.next_geometry_id_method);
        ProxiedJavaException::throw_if_any(env);

        return static_cast<uint32_t>(result);
    }

    void BridgedGPUDriver::CreateGeometry(uint32_t geometry_id,
                                          const ultralight::VertexBuffer &vertices,
                                          const ultralight::IndexBuffer &indices) {
        TemporaryJNI env;
        // TODO
    }

    void BridgedGPUDriver::UpdateGeometry(uint32_t geometry_id,
                                          const ultralight::VertexBuffer &vertices,
                                          const ultralight::IndexBuffer &indices) {
        TemporaryJNI env;
        // TODO
    }

    void BridgedGPUDriver::DestroyGeometry(uint32_t geometry_id) {
        TemporaryJNI env;

        env->CallVoidMethod(reference,
                            runtime.ultralight_gpu_driver.destroy_geometry_method,
                            static_cast<jlong>(geometry_id));
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedGPUDriver::UpdateCommandList(const ultralight::CommandList &list) {
        TemporaryJNI env;
        // TODO
    }

}
