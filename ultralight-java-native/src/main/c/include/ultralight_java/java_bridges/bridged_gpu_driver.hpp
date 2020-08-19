//
// Created by leo on 18.08.20.
//
#pragma once

#include <Ultralight/Ultralight.h>

#include "ultralight_java/util/jni_reference_wrapper.hpp"

namespace ultralight_java {

    class BridgedGPUDriver : public ultralight::GPUDriver, JNIReferenceWrapper {
    public:
        explicit BridgedGPUDriver(JNIEnv *env, jobject gpu_driver);

        void BeginSynchronize() final;

        void EndSynchronize() final;

        uint32_t NextTextureId() final;

        void CreateTexture(uint32_t texture_id,
                           ultralight::Ref<ultralight::Bitmap> bitmap) final;

        void UpdateTexture(uint32_t texture_id,
                           ultralight::Ref<ultralight::Bitmap> bitmap) final;

        void DestroyTexture(uint32_t texture_id) final;

        uint32_t NextRenderBufferId() final;

        void CreateRenderBuffer(uint32_t render_buffer_id,
                                const ultralight::RenderBuffer& buffer) final;

        void DestroyRenderBuffer(uint32_t render_buffer_id) final;

        uint32_t NextGeometryId() final;

        void CreateGeometry(uint32_t geometry_id,
                            const ultralight::VertexBuffer& vertices,
                            const ultralight::IndexBuffer& indices) final;

        void UpdateGeometry(uint32_t geometry_id,
                            const ultralight::VertexBuffer& vertices,
                            const ultralight::IndexBuffer& indices) final;

        void DestroyGeometry(uint32_t geometry_id) final;

        void UpdateCommandList(const ultralight::CommandList& list) final;

    };

}
