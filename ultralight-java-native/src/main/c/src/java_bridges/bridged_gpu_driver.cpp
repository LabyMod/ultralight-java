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
                                                 std::move(ultralight::RefPtr<ultralight::Bitmap>(std::move(bitmap)))));
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
                                                 std::move(ultralight::RefPtr<ultralight::Bitmap>(std::move(bitmap)))));
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

    void BridgedGPUDriver::CreateRenderBuffer(uint32_t render_buffer_id, const ultralight::RenderBuffer &buffer) {
        TemporaryJNI env;
        auto javaBuffer = env->NewObject(runtime.ultralight_render_buffer.clazz,
                                         runtime.ultralight_render_buffer.constructor,
                                         static_cast<jlong>(buffer.texture_id),
                                         static_cast<jlong>(buffer.width),
                                         static_cast<jlong>(buffer.height),
                                         static_cast<jboolean>(buffer.has_stencil_buffer),
                                         static_cast<jboolean>(buffer.has_depth_buffer));
        env->CallVoidMethod(reference,
                            runtime.ultralight_gpu_driver.create_render_buffer_method,
                            static_cast<jlong>(render_buffer_id),
                            javaBuffer);
        ProxiedJavaException::throw_if_any(env);
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

        jlong result = env->CallLongMethod(reference, runtime.ultralight_gpu_driver.next_geometry_id_method);
        ProxiedJavaException::throw_if_any(env);

        return static_cast<uint32_t>(result);
    }

    void BridgedGPUDriver::CreateGeometry(uint32_t geometry_id,
                                          const ultralight::VertexBuffer &vertices,
                                          const ultralight::IndexBuffer &indices) {
        TemporaryJNI env;

        auto javaIndices = env->NewObject(runtime.ultralight_indexbuffer.clazz,
                                          runtime.ultralight_indexbuffer.constructor,
                                          env->NewDirectByteBuffer(indices.data, indices.size));
        auto javaVert = env->NewObject(runtime.ultralight_vertexbuffer.clazz,
                                       runtime.ultralight_vertexbuffer.constructor,
                                       runtime.ultralight_vertexbuffer_format.constants.to_java(env, vertices.format),
                                       env->NewDirectByteBuffer(vertices.data, vertices.size));

        env->CallVoidMethod(reference,
                            runtime.ultralight_gpu_driver.create_geometry_method,
                            static_cast<jlong>(geometry_id), javaVert, javaIndices);
        ProxiedJavaException::throw_if_any(env);
    }

    void BridgedGPUDriver::UpdateGeometry(uint32_t geometry_id,
                                          const ultralight::VertexBuffer &vertices,
                                          const ultralight::IndexBuffer &indices) {
        TemporaryJNI env;

        auto javaIndices = env->NewObject(runtime.ultralight_indexbuffer.clazz,
                                          runtime.ultralight_indexbuffer.constructor,
                                          env->NewDirectByteBuffer(indices.data, indices.size));
        auto javaVert = env->NewObject(runtime.ultralight_vertexbuffer.clazz,
                                       runtime.ultralight_vertexbuffer.constructor,
                                       runtime.ultralight_vertexbuffer_format.constants.to_java(env, vertices.format),
                                       env->NewDirectByteBuffer(vertices.data, vertices.size));

        env->CallVoidMethod(reference,
                            runtime.ultralight_gpu_driver.update_geometry_method,
                            static_cast<jlong>(geometry_id), javaVert, javaIndices);
        ProxiedJavaException::throw_if_any(env);
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
        auto commands = env->NewObjectArray(list.size, runtime.ultralight_command.clazz, nullptr);

        for (int i = 0; i < list.size; i++) {
            auto& command = list.commands[i];

            auto gpuState = env->NewObject(runtime.ultralight_gpustate.clazz,
                                           runtime.ultralight_gpustate.constructor,
                                           static_cast<jlong>(command.gpu_state.viewport_width),
                                           static_cast<jlong>(command.gpu_state.viewport_height),
                                           env->NewDirectByteBuffer(command.gpu_state.transform.data, 64),
                                           static_cast<jboolean>(command.gpu_state.enable_texturing),
                                           static_cast<jboolean>(command.gpu_state.enable_blend),
                                           static_cast<jshort>(command.gpu_state.shader_type),
                                           static_cast<jlong>(command.gpu_state.render_buffer_id),
                                           static_cast<jlong>(command.gpu_state.texture_1_id),
                                           static_cast<jlong>(command.gpu_state.texture_2_id),
                                           static_cast<jlong>(command.gpu_state.texture_3_id),
                                           env->NewDirectByteBuffer(&command.gpu_state.uniform_scalar[0], 32),
                                           env->NewDirectByteBuffer(&command.gpu_state.uniform_vector->value[0], 128),
                                           static_cast<jshort>(command.gpu_state.clip_size),
                                           env->NewDirectByteBuffer(&command.gpu_state.clip->data[0], 512),
                                           static_cast<jboolean>(command.gpu_state.enable_scissor),
                                           env->NewObject(runtime.int_rect.clazz,
                                                          runtime.int_rect.bounds_constructor,
                                                          static_cast<jint>(command.gpu_state.scissor_rect.left),
                                                          static_cast<jint>(command.gpu_state.scissor_rect.top),
                                                          static_cast<jint>(command.gpu_state.scissor_rect.right),
                                                          static_cast<jint>(command.gpu_state.scissor_rect.bottom))
            );

            env->SetObjectArrayElement(commands, i, env->NewObject(runtime.ultralight_command.clazz,
                                                                   runtime.ultralight_command.constructor,
                                                                   static_cast<jshort>(command.command_type),
                                                                   gpuState,
                                                                   static_cast<jlong>(command.geometry_id),
                                                                   static_cast<jlong>(command.indices_count),
                                                                   static_cast<jlong>(command.indices_offset)));
        }

        env->CallVoidMethod(reference,
                            runtime.ultralight_gpu_driver.update_command_list_method,
                            env->NewObject(runtime.ultralight_commandlist.clazz,
                                           runtime.ultralight_commandlist.constructor,
                                           commands));
        ProxiedJavaException::throw_if_any(env);
    }

}
