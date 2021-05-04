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

package com.labymedia.ultralight.gpu;

import com.labymedia.ultralight.plugin.render.UltralightGPUDriverNative;

public class UltralightOpenGLGPUDriverNative implements UltralightGPUDriverNative {

  private final long contextHandle;
  private final long driverHandle;
  private final UltralightGPUDriverNativeUtil util;


  public UltralightOpenGLGPUDriverNative(long window, boolean msaa) {
    this.util = UltralightGPUDriverNativeUtil.getInstance();
    this.contextHandle = util.createOpenGLContext(window, msaa);
    this.driverHandle = util.getDriverFromContext(this.contextHandle);
  }

  public long getContextHandle() {
    return this.contextHandle;
  }

  @Override
  public long getDriverHandle() {
    return this.driverHandle;
  }

  @Override
  public void beginSynchronize() {
    this.util.beginSynchronize(this.driverHandle);
  }

  @Override
  public void endSynchronize() {
    this.util.endSynchronize(this.driverHandle);
  }

  @Override
  public boolean hasCommandsPending() {
    return this.util.hasCommandsPending(this.driverHandle);
  }

  @Override
  public void drawCommandList() {
    this.util.drawCommandList(this.driverHandle);
  }

  @Override
  public void bindTexture(long textureId, long texture) {
    this.util.bindTexture(this.driverHandle, textureId, texture);
  }

  @Override
  public void setActiveWindow(long window) {
    this.util.setActiveWindow(this.driverHandle, window);
  }
}
