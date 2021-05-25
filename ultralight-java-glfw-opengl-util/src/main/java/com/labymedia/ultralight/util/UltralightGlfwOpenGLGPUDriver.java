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

package com.labymedia.ultralight.util;

import com.labymedia.ultralight.gpu.UltralightOpenGLGPUDriverNative;
import org.lwjgl.glfw.GLFW;
import org.lwjgl.opengl.GL11;

/**
 * GPU Driver implementation of {@link com.labymedia.ultralight.plugin.render.UltralightGPUDriver}.
 */
public class UltralightGlfwOpenGLGPUDriver implements UltralightOpenGLGPUDriver {

  private final boolean msaa;
  private UltralightOpenGLGPUDriverNative driverNative;
  private UltralightGlfwOpenGLContext context;

  private UltralightGlfwOpenGLGPUDriver(boolean msaa) {
    this.msaa = msaa;
  }

  /**
   * Set the {@link UltralightOpenGLGPUDriverNative} instance to the {@link com.labymedia.ultralight.UltralightPlatform}.
   */
  @Override
  public void initialize(UltralightGlfwOpenGLContext context) {
    this.driverNative = new UltralightOpenGLGPUDriverNative(context.getMainWindow().getWindowHandle(), this.msaa, GLFW.Functions.GetProcAddress);
    this.context = context;
    this.context.getPlatform().setGPUDriver(this.driverNative);
  }

  /**
   * {@inheritDoc}
   */
  @Override
  public void renderTexture(UltralightGlfwOpenGLWindow window) {
    this.context.postAndWait(() -> {
      this.driverNative.setActiveWindow(window.getWindowHandle());

      UltralightRendererInstanceHolder.getRenderer().render();
      if (this.driverNative.hasCommandsPending()) {
        this.driverNative.drawCommandList();
      }
      GL11.glFinish();
    });
  }


  /**
   * {@inheritDoc}
   */
  @Override
  public void bindTexture(UltralightGlfwOpenGLWindow window) {
    this.driverNative.bindTexture(0, window.postAndWait(() -> window.getView().renderTarget().getTextureId()));
  }

  /**
   * Create a new instance of {@link UltralightGlfwOpenGLGPUDriver}.
   * Should only be called once, since the {@link UltralightGlfwOpenGLContext} can also only be instantiated once due to some Ultralight limitations.
   *
   * @param msaa define if MSAA should active for the new driver
   * @return the constructed driver instance
   */
  public static UltralightGlfwOpenGLGPUDriver create(boolean msaa) {
    return new UltralightGlfwOpenGLGPUDriver(msaa);
  }
}
