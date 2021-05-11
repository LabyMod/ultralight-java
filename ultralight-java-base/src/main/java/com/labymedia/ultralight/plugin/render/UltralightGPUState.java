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
import com.labymedia.ultralight.math.IntRect;
import com.labymedia.ultralight.math.UltralightMatrix4x4;
import com.labymedia.ultralight.math.Vec4;

/**
 * GPU state description.
 */
@NativeType("ultralight::GPUState")
public class UltralightGPUState {
    private final @Unsigned
    @NativeType("uint32_t")
    long viewportWidth;
    private final @Unsigned
    @NativeType("uint32_t")
    long viewportHeight;
    private final UltralightMatrix4x4 transformMatrix;
    private final boolean enableTexturing;
    private final boolean enableBlend;
    private final UltralightShaderType shaderType;
    private final @Unsigned
    @NativeType("uint32_t")
    long renderBufferId;
    private final @Unsigned
    @NativeType("uint32_t")
    long texture1Id;
    private final @Unsigned
    @NativeType("uint32_t")
    long texture2Id;
    private final @Unsigned
    @NativeType("uint32_t")
    long texture3Id;
    private final float[] uniformScalar;
    private final Vec4[] uniformVector;
    private final @Unsigned
    @NativeType("uint8_t")
    short clipSize;
    private final UltralightMatrix4x4[] clip;
    private final boolean enableScissor;
    private final IntRect scissorRect;

    /**
     * Constructs a new GPU state and sets its values.
     *
     * @param viewportWidth   Viewport width in pixels
     * @param viewportHeight  Viewport height in pixels
     * @param transformMatrix Transform matrix for the orthographic projection
     * @param enableTexturing Whether or not we should enable texturing for the current draw command
     * @param enableBlend     Whether or not we should enable blending for the current draw command
     * @param shaderType      The vertex/pixel shader program pair to use for the current draw command
     * @param renderBufferId  The render buffer to use for the current draw command
     * @param texture1Id      The texture id to bind to slot #1 (Will be 0 if none)
     * @param texture2Id      he texture id to bind to slot #2 (Will be 0 if none)
     * @param texture3Id      The texture id to bind to slot #3 (Will be 0 if none)
     * @param uniformScalar   Uniform scalar passed to pixel shader
     * @param uniformVector   Uniform vector passed to pixel shader
     * @param clipSize        Clip size passed to pixel shader
     * @param clip            Clip passed to pixel shader
     * @param enableScissor   Whether or not scissor testing should be used for the current draw command
     * @param scissorRect     The scissor rect to use for scissor testing (units in pixels)
     */
    @NativeCall
    public UltralightGPUState(
            @Unsigned
            @NativeType("uint32_t") long viewportWidth,
            @Unsigned
            @NativeType("uint32_t") long viewportHeight,
            UltralightMatrix4x4 transformMatrix,
            boolean enableTexturing,
            boolean enableBlend,
            UltralightShaderType shaderType,
            @Unsigned @NativeType("uint32_t") long renderBufferId,
            @Unsigned @NativeType("uint32_t") long texture1Id,
            @Unsigned @NativeType("uint32_t") long texture2Id,
            @Unsigned @NativeType("uint32_t") long texture3Id,
            float[] uniformScalar,
            Vec4[] uniformVector,
            @Unsigned @NativeType("uint8_t") short clipSize,
            UltralightMatrix4x4[] clip,
            boolean enableScissor,
            IntRect scissorRect
    ) {
        this.viewportWidth = viewportWidth;
        this.viewportHeight = viewportHeight;
        this.transformMatrix = transformMatrix;
        this.enableTexturing = enableTexturing;
        this.enableBlend = enableBlend;
        this.shaderType = shaderType;
        this.renderBufferId = renderBufferId;
        this.texture1Id = texture1Id;
        this.texture2Id = texture2Id;
        this.texture3Id = texture3Id;
        this.uniformScalar = uniformScalar;
        this.uniformVector = uniformVector;
        this.clipSize = clipSize;
        this.clip = clip;
        this.enableScissor = enableScissor;
        this.scissorRect = scissorRect;
    }

    /**
     * Retrieves the viewport width in pixels.
     *
     * @return The viewport width in pixels
     */
    @Unsigned
    @NativeType("uint32_t")
    public long getViewportWidth() {
        return viewportWidth;
    }

    /**
     * Retrieves the viewport height in pixels.
     *
     * @return The viewport height in pixels
     */
    @Unsigned
    @NativeType("uint32_t")
    public long getViewportHeight() {
        return viewportHeight;
    }

    /**
     * Retrieves the transform matrix for the orthographic projection.
     * <p>
     * You should multiply this with the screen-space orthographic projection matrix then pass to the vertex shader.
     *
     * @return The transform matrix for the orthographic projection
     */
    public UltralightMatrix4x4 getTransformMatrix() {
        return transformMatrix;
    }

    /**
     * Determines whether or not texturing should be enabled.
     *
     * @return {@code true} if texturing should be enabled, {@code false} otherwise
     */
    public boolean shouldEnableTexturing() {
        return enableTexturing;
    }

    /**
     * Determines whether blending should be enabled.
     * <p>
     * If blending is disabled, any drawn pixels should overwrite existing. Mainly used so we can modify alpha values of
     * the RenderBuffer during scissored clears.
     *
     * @return {@code true} if blending should be enabled, {@code false} otherwise
     */
    public boolean shouldEnableBlend() {
        return enableBlend;
    }

    /**
     * Determines which pixel/vertex shader pair to use.
     *
     * @return The type of pixel/vertex shader pair to use
     */
    public UltralightShaderType getShaderType() {
        return shaderType;
    }

    /**
     * Retrieves the id of the render buffer to use.
     *
     * @return The id of the render buffer to use
     */
    @Unsigned
    @NativeType("uint32_t")
    public long getRenderBufferId() {
        return renderBufferId;
    }

    /**
     * Retrieves the id of the texture to bind to slot #1.
     *
     * @return The id of the texture to bind to slot #1, or {@code 0}, if none
     */
    @Unsigned
    @NativeType("uint32_t")
    public long getTexture1Id() {
        return texture1Id;
    }

    /**
     * Retrieves the id of the texture to bind to slot #2.
     *
     * @return The id of the texture to bind to slot #2, or {@code 0}, if none
     */
    @Unsigned
    @NativeType("uint32_t")
    public long getTexture2Id() {
        return texture2Id;
    }

    /**
     * Retrieves the id of the texture to bind to slot #3.
     *
     * @return The id of the texture to bind to slot #3, or {@code 0}, if none
     */
    @Unsigned
    @NativeType("uint32_t")
    public long getTexture3Id() {
        return texture3Id;
    }

    /**
     * Retrieves the values of the uniform scalar passed to the pixel shader.
     *
     * @return The values of the uniform scalar passed to the pixel shader
     */
    public float[] getUniformScalar() {
        return uniformScalar;
    }

    /**
     * Retrieves the values of the uniform vector passed to the pixel shader.
     *
     * @return The values of the uniform vector passed to the pixel shader
     */
    public Vec4[] getUniformVector() {
        return uniformVector;
    }

    /**
     * Retrieves the clip size passed to the pixel shader.
     *
     * @return The clip size passed to the pixel shader
     */
    @Unsigned
    @NativeType("uint8_t")
    public short getClipSize() {
        return clipSize;
    }

    /**
     * Retrieves the clip passed to the pixel shader.
     *
     * @return The clip passed to the pixel shader
     */
    public UltralightMatrix4x4[] getClip() {
        return clip;
    }

    /**
     * Determines whether scissor testing should be enabled.
     *
     * @return {@code true} if scissor testing should be enabled, {@code false} otherwise
     */
    public boolean shouldEnableScissor() {
        return enableScissor;
    }

    /**
     * Retrieves the rect to use for scissor testing in pixels.
     *
     * @return The rect to use for scissor testing
     */
    public IntRect getScissorRect() {
        return scissorRect;
    }
}
