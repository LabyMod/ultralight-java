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

package com.labymedia.ultralight.example.simple;

import com.labymedia.ultralight.config.FontHinting;
import com.labymedia.ultralight.config.UltralightConfig;
import com.labymedia.ultralight.example.base.UltralightExampleBase;
import com.labymedia.ultralight.example.base.UltralightExampleConfiguration;
import com.labymedia.ultralight.gpu.UltralightOpenGLGPUDriverNative;
import com.labymedia.ultralight.util.UltralightGlfwOpenGLContext;
import com.labymedia.ultralight.util.UltralightGlfwOpenGLGPUDriver;
import com.labymedia.ultralight.util.UltralightGlfwOpenGLWindow;
import org.lwjgl.glfw.GLFW;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.MemoryUtil;

import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL20.glUseProgram;

public class SimpleUltralightExample extends UltralightExampleBase {

    private UltralightOpenGLGPUDriverNative driverNative;
    private UltralightGlfwOpenGLContext context;

    @Override
    public void configure(UltralightExampleConfiguration ultralightExampleConfiguration) {
        //Setup GPU accelerated context
        this.context = UltralightGlfwOpenGLContext.create(500, 500, "Test123", UltralightGlfwOpenGLGPUDriver.create(false))
                .postAndWait(context -> {
                    context.getPlatform().setConfig(
                            new UltralightConfig()
                                    .forceRepaint(true)
                                    .resourcePath("./resources/")
                                    .fontHinting(FontHinting.SMOOTH)
                    );
                    context.getPlatform().usePlatformFontLoader();
                });
        context.getMainWindow().show();
    }

    @Override
    public void begin() {
        this.context
                .post(() -> {
                    UltralightGlfwOpenGLWindow mainWindow = context.getMainWindow();
                    mainWindow.loadUrl("https://google.de");
                    while (!mainWindow.shouldClose()) {
                        this.context
                                .updateJavaScript();
                        mainWindow
                                .makeContext()
                                .updateWebContent();
//                        renderWindow(mainWindow);
                    }
                    mainWindow.close();
                });

        try {
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        long minecraftWindow = GLFW.glfwCreateWindow(500, 500, "Title123", MemoryUtil.NULL, this.context.getMainWindow().getWindowHandle());
        GLFW.glfwShowWindow(minecraftWindow);
        GLFW.glfwMakeContextCurrent(minecraftWindow);

        System.out.println(context.getMainWindow().getView().getDeviceScale());
        GL.createCapabilities();
        while (true){
            GLFW.glfwMakeContextCurrent(minecraftWindow);
            GLFW.glfwPollEvents();

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(1f, 1f, 1f, 1f);

            context.getMainWindow()
                    .bindTexture();

            glUseProgram(0);
            glEnable(GL_TEXTURE_2D);
            glColor3f(1f, 1f, 1f);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0, 500, 500, 0, -1, 1);
            glViewport(0, 0, 500, 500);

            glBegin(GL_POLYGON);

            glTexCoord2f(0, 0);
            glVertex3f(0, 0, 0);

            glTexCoord2f(0, 1);
            glVertex3f(0, 500, 0);

            glTexCoord2f(1, 1);
            glVertex3f(500, 500, 0);

            glTexCoord2f(1, 0);
            glVertex3f(500, 0, 0);

            glEnd();

            GLFW.glfwSwapBuffers(minecraftWindow);
        }


    }

    private void renderWindow(UltralightGlfwOpenGLWindow window) {
        window.makeContext();

        GLFW.glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1f, 1f, 1f, 1f);

        window
                .updateWebContent()
                .bindTexture();

        glUseProgram(0);
        glEnable(GL_TEXTURE_2D);
        glColor3f(1f, 1f, 1f);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, window.getWidth(), window.getHeight(), 0, -1, 1);
        glViewport(0, 0, window.getWidth(), window.getHeight());

        glBegin(GL_POLYGON);

        glTexCoord2f(0, 0);
        glVertex3f(0, 0, 0);

        glTexCoord2f(0, 1);
        glVertex3f(0, window.getHeight(), 0);

        glTexCoord2f(1, 1);
        glVertex3f(window.getWidth(), window.getHeight(), 0);

        glTexCoord2f(1, 0);
        glVertex3f(window.getWidth(), 0, 0);

        glEnd();
        window.swapBuffers();
    }

    public static void main(String[] args) {
        new SimpleUltralightExample();
    }
}
