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

import com.labymedia.ultralight.annotation.NativeCall;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.annotation.Unsigned;

/**
 * RenderBuffer description.
 *
 * @see UltralightGPUDriver#createRenderBuffer(long, UltralightRenderBuffer)
 */
@NativeType("ultralight::RenderBuffer")
public class UltralightRenderBuffer {
    private final @Unsigned
    @NativeType("uint32_t")
    long textureId;
    private final @Unsigned
    @NativeType("uint32_t")
    long width;
    private final @Unsigned
    @NativeType("uint32_t")
    long height;
    private final boolean hasStencilBuffer;
    private final boolean hasDepthBuffer;

    /**
     * Constructs a new {@link UltralightRenderBuffer}.
     *
     * @param textureId        The backing texture for this {@link UltralightRenderBuffer}
     * @param width            The width of the RenderBuffer texture
     * @param height           The height of the RenderBuffer texture
     * @param hasStencilBuffer Currently unused, always {@code false}
     * @param hasDepthBuffer   Currently unused, always {@code false}
     */
    @NativeCall
    public UltralightRenderBuffer(
            @Unsigned @NativeType("uint32_t") long textureId,
            @Unsigned @NativeType("uint32_t") long width,
            @Unsigned @NativeType("uint32_t") long height,
            boolean hasStencilBuffer,
            boolean hasDepthBuffer
    ) {
        this.textureId = textureId;
        this.width = width;
        this.height = height;
        this.hasStencilBuffer = hasStencilBuffer;
        this.hasDepthBuffer = hasDepthBuffer;
    }

    /**
     * Retrieves the id of the backing texture of this render buffer.
     *
     * @return The id of the backing texture
     */
    public long getTextureId() {
        return textureId;
    }

    /**
     * Retrieves the width of this render buffer.
     *
     * @return The width of this render buffer
     */
    public long getWidth() {
        return width;
    }

    /**
     * Retrieves the height of this render buffer.
     *
     * @return The height of this render buffer
     */
    public long getHeight() {
        return height;
    }

    /**
     * Determines whether this render buffer has a stencil buffer.
     *
     * @return Currently always {@code false}
     */
    public boolean hasStencilBuffer() {
        return hasStencilBuffer;
    }

    /**
     * Determines whether this render buffer has a depth buffer.
     *
     * @return Currently always {@code false}
     */
    public boolean hasDepthBuffer() {
        return hasDepthBuffer;
    }
}
