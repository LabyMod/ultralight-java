package net.janrupf.ultralight.plugin.render;

import net.janrupf.ultralight.annotation.NativeType;

@NativeType("ultralight::Command")
public class UltralightCommand {

    private final @NativeType("uint8_t") short commandType;
    private final @NativeType("ultralight::GPUState") UltralightGPUState gpuState;

    private final @NativeType("uint32_t") long geometryId;
    private final @NativeType("uint32_t") long indicesCount;
    private final @NativeType("uint32_t") long indicesOffset;

    public UltralightCommand(short commandType,
                             UltralightGPUState gpuState,
                             long geometryId,
                             long indicesCount,
                             long indicesOffset) {
        this.commandType = commandType;
        this.gpuState = gpuState;
        this.geometryId = geometryId;
        this.indicesCount = indicesCount;
        this.indicesOffset = indicesOffset;
    }

    public short getCommandType() {
        return commandType;
    }

    public UltralightGPUState getGpuState() {
        return gpuState;
    }

    public long getGeometryId() {
        return geometryId;
    }

    public long getIndicesCount() {
        return indicesCount;
    }

    public long getIndicesOffset() {
        return indicesOffset;
    }
}
