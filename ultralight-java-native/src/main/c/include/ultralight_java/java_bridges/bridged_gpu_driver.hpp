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
