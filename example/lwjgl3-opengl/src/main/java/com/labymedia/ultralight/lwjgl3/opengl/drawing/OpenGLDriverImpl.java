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

package com.labymedia.ultralight.lwjgl3.opengl.drawing;

import com.labymedia.ultralight.plugin.render.UltralightCommand;
import com.labymedia.ultralight.plugin.render.UltralightCommandList;
import com.labymedia.ultralight.plugin.render.UltralightGPUDriverImpl;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public abstract class OpenGLDriverImpl implements UltralightGPUDriverImpl {

  private long nextTextureId = 1;
  private long nextRenderBufferId = 1;
  private long nextGeometryId = 1;
  private List<UltralightCommand> commandList = new ArrayList<>();
  private int batchCount;

  private static final int KCOMMMANDTYPE_CLEARRENDERBUFFER = 0;
  private static final int KCOMMMANDTYPE_DRAWGEOMETRY = 1;

  @Override
  public boolean hasCommandsPending() {
    return !this.commandList.isEmpty();
  }

  @Override
  public void drawCommandList() {
    if (this.commandList.isEmpty()) {
      return;
    }

    this.batchCount = 0;

    for (UltralightCommand cmd : this.commandList) {
      if (cmd.commandType == KCOMMMANDTYPE_DRAWGEOMETRY) {
        this.drawGeometry(cmd.geometryId, cmd.indicesCount, cmd.indicesOffset, cmd.gpuState);
      } else if (cmd.commandType == KCOMMMANDTYPE_CLEARRENDERBUFFER) {
        this.clearRenderBuffer(cmd.gpuState.renderBufferId);
      }
      this.batchCount++;
    }

    this.commandList.clear();
  }

  @Override
  public int batchCount() {
    return this.batchCount;
  }

  @Override
  public long nextTextureId() {
    return this.nextTextureId++;
  }

  @Override
  public long nextRenderBufferId() {
    return this.nextRenderBufferId++;
  }

  @Override
  public long nextGeometryId() {
    return this.nextGeometryId++;
  }

  @Override
  public void updateCommandList(UltralightCommandList list) {
    if (list.data.length > 0) {
      this.commandList.clear();
      this.commandList.addAll(Arrays.asList(list.data));
    }
  }
}
