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
import com.labymedia.ultralight.bitmap.UltralightBitmapFormat;

/**
 * Rendering details for a View, to be used with your own {@link UltralightGPUDriver}.
 * <p>
 * When using your own {@link UltralightGPUDriver}, each {@link com.labymedia.ultralight.UltralightView} is rendered to
 * an offscreen texture that you can display on a 3D quad in your application. This struct provides all the details you
 * need to display the corresponding texture in your application.
 */
public class UltralightRenderTarget {
    private final boolean isEmpty;
    private final @Unsigned
    @NativeType("uint32_t")
    long width;
    private final @Unsigned
    @NativeType("uint32_t")
    long height;
    private final @Unsigned
    @NativeType("uint32_t")
    long textureId;
    private final @Unsigned
    @NativeType("uint32_t")
    long textureWidth;
    private final @Unsigned
    @NativeType("uint32_t")
    long textureHeight;
    private final UltralightBitmapFormat textureFormat;
    private final float[] uvCoords;
    private final @Unsigned
    @NativeType("uint32_t")
    long renderBufferId;

    /**
     * Constructs a new {@link UltralightRenderTarget}.
     *
     * @param isEmpty        Whether this target is empty (a null texture)
     * @param width          The viewport width (in device coordinates)
     * @param height         The viewport height (in device coordinates)
     * @param textureId      The gpu driver specific texture id
     * @param textureWidth   The texture width in pixels, may be padded
     * @param textureHeight  The texture height in pixels, may be padded
     * @param textureFormat  The pixel format of the texture
     * @param uvCoords       The UV coordinates of the texture
     * @param renderBufferId The gpu driver specific render buffer id
     */
    @NativeCall
    public UltralightRenderTarget(
            boolean isEmpty,
            @Unsigned @NativeType("uint32_t") long width,
            @Unsigned @NativeType("uint32_t") long height,
            @Unsigned @NativeType("uint32_t") long textureId,
            @Unsigned @NativeType("uint32_t") long textureWidth,
            @Unsigned @NativeType("uint32_t") long textureHeight,
            UltralightBitmapFormat textureFormat,
            float[] uvCoords,
            @Unsigned @NativeType("uint32_t") long renderBufferId
    ) {
        this.isEmpty = isEmpty;
        this.width = width;
        this.height = height;
        this.textureId = textureId;
        this.textureWidth = textureWidth;
        this.textureHeight = textureHeight;
        this.textureFormat = textureFormat;
        this.uvCoords = uvCoords;
        this.renderBufferId = renderBufferId;
    }

    /**
     * Determines whether this target is empty (a null texture).
     *
     * @return {@code true} if this target is a empty, {@code false} otherwise
     */
    public boolean isEmpty() {
        return isEmpty;
    }

    /**
     * Retrieves the viewport width.
     *
     * @return The viewport width in pixels
     */
    public long getWidth() {
        return width;
    }

    /**
     * Retrieves the viewport height.
     *
     * @return The viewport height in pixels
     */
    public long getHeight() {
        return height;
    }

    /**
     * Retrieves the gpu driver specific texture id.
     * <p>
     * The texture needs to be bound by mapping the internal id back to the real id using your gpu driver implementation.
     *
     * @return The gpu driver specific texture id
     */
    public long getTextureId() {
        return textureId;
    }

    /**
     * Retrieves the possibly padded texture width.
     *
     * @return The width of the texture
     */
    public long getTextureWidth() {
        return textureWidth;
    }

    /**
     * Retrieves the possibly padded texture height.
     *
     * @return The height of the texture
     */
    public long getTextureHeight() {
        return textureHeight;
    }

    /**
     * Retrieves the format of the texture.
     *
     * @return The format of the texture
     */
    public UltralightBitmapFormat getTextureFormat() {
        return textureFormat;
    }

    /**
     * Retrieves the UV coordinates of the texture.
     * <p>
     * This is needed since the texture may be padded.
     *
     * @return The UV coordinates of the texture
     */
    public float[] getUvCoords() {
        return uvCoords;
    }

    /**
     * Retrieves the gpu driver specific render buffer id.
     *
     * @return The gpu driver specific render buffer id
     */
    public long getRenderBufferId() {
        return renderBufferId;
    }
}
