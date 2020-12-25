/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
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

package com.labymedia.ultralight.plugin.render;

import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.bitmap.UltralightBitmap;

@NativeType("ultralight::GPUDriver")
public interface UltralightGPUDriver {

    void beginSynchronize();

    void endSynchronize();

    @NativeType("uint32_t")
    long nextTextureId();

    void createTexture(@NativeType("uint32_t") long textureId,
                       @NativeType("ultralight::Ref<ultralight::Bitmap>") UltralightBitmap bitmap);

    void updateTexture(@NativeType("uint32_t") long textureId,
                       @NativeType("ultralight::Ref<ultralight::Bitmap>") UltralightBitmap bitmap);

    void destroyTexture(@NativeType("uint32_t") long textureId);

    @NativeType("uint32_t")
    long nextRenderBufferId();

    void createRenderBuffer(@NativeType("uint32_t") long renderBufferId,
                            @NativeType("const ultralight::RenderBuffer &") UltralightRenderBuffer buffer);

    void destroyRenderBuffer(@NativeType("uint32_t") long renderBufferId);

    @NativeType("uint32_t")
    long nextGeometryId();

    void createGeometry(@NativeType("uint32_t") long geometryId,
                        @NativeType("const ultralight::VertexBuffer &") final UltralightVertexBuffer vertices,
                        @NativeType("const ultralight::IndexBuffer &") final UltralightIndexBuffer indices);

    void updateGeometry(@NativeType("uint32_t") long geometryId,
                        @NativeType("const ultralight::VertexBuffer &") final UltralightVertexBuffer vertices,
                        @NativeType("const ultralight::IndexBuffer &") final UltralightIndexBuffer indices);

    void destroyGeometry(@NativeType("uint32_t") long geometryId);

    void updateCommandList(@NativeType("const ultralight::CommandList &") final UltralightCommandList list);

}
