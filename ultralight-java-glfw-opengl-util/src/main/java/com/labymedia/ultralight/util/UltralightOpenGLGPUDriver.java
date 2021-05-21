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

/**
 * Defines the minimal logic to generate an OpenGL texture from the current
 */
public interface UltralightOpenGLGPUDriver {

    /**
     * Set driver specific Ultralight Settings to a given context
     *
     * @param context the context to initialize
     */
    void initialize(UltralightGlfwOpenGLContext context);

    /**
     * Update web content and render it to an OpenGL texture.
     *
     * @param window the window to reload and render
     */
    void renderTexture(UltralightGlfwOpenGLWindow window);

    /**
     * Bind the latest rendered texture of a given window to the current OpenGL context.
     *
     * @param window the window to retrieve texture from
     */
    void bindTexture(UltralightGlfwOpenGLWindow window);

}
