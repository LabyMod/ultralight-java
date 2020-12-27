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

@NativeType("ultralight::Command")
public class UltralightCommand {
    public final @NativeType("uint8_t")
    short commandType;
    public final @NativeType("ultralight::GPUState")
    UltralightGPUState gpuState;

    public final @NativeType("uint32_t")
    long geometryId;
    public final @NativeType("uint32_t")
    long indicesCount;
    public final @NativeType("uint32_t")
    long indicesOffset;

    public UltralightCommand(short commandType, UltralightGPUState gpuState, long geometryId, long indicesCount, long indicesOffset) {
        this.commandType = commandType;
        this.gpuState = gpuState;
        this.geometryId = geometryId;
        this.indicesCount = indicesCount;
        this.indicesOffset = indicesOffset;
    }
}
