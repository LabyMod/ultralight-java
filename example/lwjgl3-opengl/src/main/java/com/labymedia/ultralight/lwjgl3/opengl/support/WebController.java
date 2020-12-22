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

package com.labymedia.ultralight.lwjgl3.opengl.support;

import com.labymedia.ultralight.UltralightPlatform;
import com.labymedia.ultralight.UltralightRenderer;
import com.labymedia.ultralight.UltralightView;
import com.labymedia.ultralight.bitmap.UltralightBitmap;
import com.labymedia.ultralight.bitmap.UltralightBitmapSurface;
import com.labymedia.ultralight.config.FontHinting;
import com.labymedia.ultralight.config.UltralightConfig;
import com.labymedia.ultralight.lwjgl3.opengl.input.CursorAdapter;
import com.labymedia.ultralight.lwjgl3.opengl.input.InputAdapter;
import com.labymedia.ultralight.lwjgl3.opengl.input.ClipboardAdapter;
import com.labymedia.ultralight.lwjgl3.opengl.listener.ExampleLoadListener;
import com.labymedia.ultralight.lwjgl3.opengl.listener.ExampleViewListener;
import com.labymedia.ultralight.math.IntRect;

import java.nio.ByteBuffer;

import static org.lwjgl.opengl.GL20.*;

/**
 * Class used for controlling the WebGUI rendered on top of the OpenGL GUI.
 */
public class WebController {
    private final UltralightPlatform platform;
    private final UltralightRenderer renderer;
    private final UltralightView view;
    private final ExampleViewListener viewListener;
    private final ExampleLoadListener loadListener;
    private final InputAdapter inputAdapter;

    private int glTexture;

    /**
     * Constructs a new {@link WebController} and retrieves the platform.
     *
     * @param cursorManager Cursor manager for callbacks on cursor changes
     */
    public WebController(CursorAdapter cursorManager) {
        this.platform = UltralightPlatform.instance();

        this.platform.setConfig(
                new UltralightConfig()
                        .resourcePath("./resources/")
                        .fontHinting(FontHinting.NORMAL)
                        .deviceScale(1.0)
        );
        this.platform.usePlatformFontLoader();
        this.platform.setFileSystem(new ExampleFileSystem());
        this.platform.setLogger(new ExampleLogger());
        this.platform.setClipboard(new ClipboardAdapter());

        this.renderer = UltralightRenderer.create();
        this.renderer.logMemoryUsage();

        this.view = renderer.createView(300, 300, true);
        this.viewListener = new ExampleViewListener(cursorManager);
        this.view.setViewListener(viewListener);
        this.loadListener = new ExampleLoadListener(view);
        this.view.setLoadListener(loadListener);
        this.glTexture = -1;

        this.inputAdapter = new InputAdapter(view);
    }

    /**
     * Retrieves the input adapter of this web controller.
     *
     * @return The input adapter of this web controller
     */
    public InputAdapter getInputAdapter() {
        return inputAdapter;
    }

    /**
     * Loads the specified URL into this controller.
     *
     * @param url The URL to load
     */
    public void loadURL(String url) {
        this.view.loadURL(url);
    }

    /**
     * Updates and renders the renderer
     */
    public void update() {
        this.renderer.update();
        this.renderer.render();
    }

    /**
     * Resizes the web view.
     *
     * @param width  The new view width
     * @param height The new view height
     */
    public void resize(int width, int height) {
        this.view.resize(width, height);
    }

    /**
     * Render the current image using OpenGL
     */
    public void render() {
        if(glTexture == -1) {
            createGLTexture();
        }

        UltralightBitmapSurface surface = (UltralightBitmapSurface) this.view.surface();
        UltralightBitmap bitmap = surface.bitmap();

        int width = (int) view.width();
        int height = (int) view.height();

        // Prepare OpenGL for 2D textures and bind our texture
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, this.glTexture);

        IntRect dirtyBounds = surface.dirtyBounds();

        if(dirtyBounds.isValid()) {
            ByteBuffer imageData = bitmap.lockPixels();

            glPixelStorei(GL_UNPACK_ROW_LENGTH, (int) bitmap.rowBytes() / 4);
            if(dirtyBounds.width() == width && dirtyBounds.height() == height) {
                // Update full image
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, imageData);
                glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
            } else {
                // Update partial image
                int x = dirtyBounds.x();
                int y = dirtyBounds.y();
                int dirtyWidth = dirtyBounds.width();
                int dirtyHeight = dirtyBounds.height();
                int startOffset = (int) ((y * bitmap.rowBytes()) + x * 4);

                glTexSubImage2D(
                        GL_TEXTURE_2D,
                        0,
                        x, y, dirtyWidth, dirtyHeight,
                        GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV,
                        (ByteBuffer) imageData.position(startOffset));
            }
            glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);

            bitmap.unlockPixels();
            surface.clearDirtyBounds();
        }

        // Set up the OpenGL state for rendering of a fullscreen quad
        glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        glOrtho(0, this.view.width(), this.view.height(), 0, -1, 1);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        // Disable lighting and scissoring, they could mess up th renderer
        glLoadIdentity();
        glDisable(GL_LIGHTING);
        glDisable(GL_SCISSOR_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Make sure we draw with a neutral color
        // (so we don't mess with the color channels of the image)
        glColor4f(1, 1, 1, 1f);

        glBegin(GL_QUADS);

        // Lower left corner, 0/0 on the screen space, and 0/0 of the image UV
        glTexCoord2i(0, 0);
        glVertex2f(0, 0);

        // Upper left corner
        glTexCoord2f(0, 1);
        glVertex2i(0, height);

        // Upper right corner
        glTexCoord2f(1, 1);
        glVertex2i(width, height);

        // Lower right corner
        glTexCoord2f(1, 0);
        glVertex2i(width, 0);

        glEnd();

        glBindTexture(GL_TEXTURE_2D, 0);

        // Restore OpenGL state
        glPopMatrix();
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);

        glDisable(GL_TEXTURE_2D);
        glPopAttrib();

    }

    /**
     * Sets up the OpenGL texture for rendering
     */
    private void createGLTexture() {
        glEnable(GL_TEXTURE_2D);
        this.glTexture = glGenTextures();
        glBindTexture(GL_TEXTURE_2D, this.glTexture);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}
