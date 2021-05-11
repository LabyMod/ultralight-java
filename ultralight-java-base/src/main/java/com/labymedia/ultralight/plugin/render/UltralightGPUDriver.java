/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 - 2021 LabyMedia and contributors
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

import com.labymedia.ultralight.UltralightRenderer;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.annotation.Unsigned;
import com.labymedia.ultralight.bitmap.UltralightBitmap;

/**
 * GPUDriver interface, dispatches GPU calls to the native driver.
 * <p>
 * If you are using {@link UltralightRenderer#create()}, you will need to provide your own
 * <p>
 * implementation of this class if you have enabled the GPU renderer in the
 * Config.
 *
 * @see com.labymedia.ultralight.UltralightPlatform#setGPUDriver(UltralightGPUDriver)
 */
@NativeType("ultralight::GPUDriver")
public interface UltralightGPUDriver {
    /**
     * Called before any commands are dispatched during a frame.
     */
    void beginSynchronize();

    /**
     * Called after any commands are dispatched during a frame.
     */
    void endSynchronize();

    /**
     * Get the next available texture ID.
     *
     * @return The next available texture ID
     */
    @Unsigned
    @NativeType("uint32_t")
    long nextTextureId();

    /**
     * Create a texture with a certain ID and optional bitmap.
     * <p>
     * If the Bitmap is empty {@link UltralightBitmap#isEmpty()}, then a RTT Texture should be created instead. This
     * will be used as a backing texture for a new RenderBuffer.
     *
     * @param textureId The id of the texture to create
     * @param bitmap    The bitmap to use, if any
     */
    void createTexture(@Unsigned @NativeType("uint32_t") long textureId, @NativeType(
            "ultralight::Ref<ultralight::Bitmap>") UltralightBitmap bitmap);

    /**
     * Update an existing non-RTT texture with new bitmap data.
     *
     * @param textureId The id of the texture ot update
     * @param bitmap    The new bitmap to use
     */
    void updateTexture(@Unsigned @NativeType("uint32_t") long textureId, @NativeType(
            "ultralight::Ref<ultralight::Bitmap>") UltralightBitmap bitmap);

    /**
     * Destroy a texture.
     *
     * @param textureId The id of the texture to destroy
     */
    void destroyTexture(@Unsigned @NativeType("uint32_t") long textureId);

    /**
     * Generate the next available render buffer ID.
     *
     * @return The next available render buffer ID
     */
    @Unsigned
    @NativeType("uint32_t")
    long nextRenderBufferId();

    /**
     * Create a render buffer with certain ID and buffer description.
     *
     * @param renderBufferId The id of the buffer to create
     * @param buffer         The description of the buffer to create
     */
    void createRenderBuffer(@Unsigned @NativeType("uint32_t") long renderBufferId, @NativeType(
            "ultralight::RenderBuffer") UltralightRenderBuffer buffer);

    /**
     * Destroy a render buffer.
     *
     * @param renderBufferId The render buffer to destroy
     */
    void destroyRenderBuffer(@Unsigned @NativeType("uint32_t") long renderBufferId);

    /**
     * Generate the next available geometry ID.
     *
     * @return The next available geometry ID
     */
    @Unsigned
    @NativeType("uint32_t")
    long nextGeometryId();

    /**
     * Create geometry with certain ID and vertex/index data.
     *
     * @param geometryId The id of the geometry to create
     * @param vertices   The vertex data of the geometry
     * @param indices    The index data of the geometry
     */
    void createGeometry(@Unsigned @NativeType("uint32_t") long geometryId, @NativeType(
            "ultralight::VertexBuffer") UltralightVertexBuffer vertices, @NativeType(
            "ultralight::IndexBuffer") UltralightIndexBuffer indices);

    /**
     * Update existing geometry with new vertex/index data.
     *
     * @param geometryId The id of the geometry to update
     * @param vertices   The new vertex data of the geometry
     * @param indices    The The index data of the geometry
     */
    void updateGeometry(@Unsigned @NativeType("uint32_t") long geometryId, @NativeType(
            "ultralight::VertexBuffer") UltralightVertexBuffer vertices, @NativeType(
            "ultralight::IndexBuffer") UltralightIndexBuffer indices);

    /**
     * Destroy geometry.
     *
     * @param geometryId The id of the geometry to destroy
     */
    void destroyGeometry(@Unsigned @NativeType("uint32_t") long geometryId);

    /**
     * Update command list.
     *
     * @param list The new list of commands
     */
    void updateCommandList(UltralightCommand[] list);
}
