package net.janrupf.ultralight.plugin.render;

import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.math.IntRect;

@NativeType("ultralight::GPUState")
public class UltralightGPUState {

    private final @NativeType("uint32_t") long viewportWidth;
    private final @NativeType("uint32_t") long viewportHeight;

    private final @NativeType("ultralight::Matrix4x4 []") float[][] transform;

    private final @NativeType("bool") boolean enableTexturing;
    private final @NativeType("bool") boolean enableBlend;

    private final @NativeType("uint8_t") short shaderType;

    private final @NativeType("uint32_t") long renderBufferId;
    private final @NativeType("uint32_t") long texture1Id;
    private final @NativeType("uint32_t") long texture2Id;
    private final @NativeType("uint32_t") long texture3Id;

    private final @NativeType("float []") float[] uniformScalar;
    private final @NativeType("ultralight::vec4 []") float[][] uniformVector;
    private final @NativeType("uint8_t") short clipSize;
    private final @NativeType("ultralight::Matrix4x4 []") float[][] clip;

    private final @NativeType("bool") boolean enableScissor;

    private final @NativeType("IntRect") IntRect scissorRect;

    public UltralightGPUState(long viewportWidth,
                              long viewportHeight,
                              float[][] transform,
                              boolean enableTexturing,
                              boolean enableBlend,
                              short shaderType,
                              long renderBufferId,
                              long texture1Id,
                              long texture2Id,
                              long texture3Id,
                              float[] uniformScalar,
                              float[][] uniformVector,
                              short clipSize,
                              float[][] clip,
                              boolean enableScissor,
                              IntRect scissorRect) {
        this.viewportWidth = viewportWidth;
        this.viewportHeight = viewportHeight;
        this.transform = transform;
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

    public long getViewportWidth() {
        return viewportWidth;
    }

    public long getViewportHeight() {
        return viewportHeight;
    }

    public float[][] getTransform() {
        return transform;
    }

    public boolean isEnableTexturing() {
        return enableTexturing;
    }

    public boolean isEnableBlend() {
        return enableBlend;
    }

    public short getShaderType() {
        return shaderType;
    }

    public long getRenderBufferId() {
        return renderBufferId;
    }

    public long getTexture1Id() {
        return texture1Id;
    }

    public long getTexture2Id() {
        return texture2Id;
    }

    public long getTexture3Id() {
        return texture3Id;
    }

    public float[] getUniformScalar() {
        return uniformScalar;
    }

    public float[][] getUniformVector() {
        return uniformVector;
    }

    public short getClipSize() {
        return clipSize;
    }

    public float[][] getClip() {
        return clip;
    }

    public boolean isEnableScissor() {
        return enableScissor;
    }

    public IntRect getScissorRect() {
        return scissorRect;
    }
}
