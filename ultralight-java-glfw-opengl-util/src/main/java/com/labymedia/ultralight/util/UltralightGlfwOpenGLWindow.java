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

import com.labymedia.ultralight.UltralightPlatform;
import com.labymedia.ultralight.UltralightView;
import com.labymedia.ultralight.config.UltralightViewConfig;
import com.labymedia.ultralight.plugin.render.UltralightGPUDriver;
import com.labymedia.ultralight.plugin.render.UltralightGPUDriverNative;
import org.lwjgl.glfw.GLFW;
import org.lwjgl.system.MemoryUtil;

import java.util.concurrent.Callable;
import java.util.concurrent.Future;
import java.util.function.Consumer;

import static org.lwjgl.glfw.GLFW.*;

/**
 * Utility class to handle GLFW window creation and Ultralight logic.
 * Represents one open glfw-window/{@link UltralightView} per instance.
 */
public class UltralightGlfwOpenGLWindow {

    private final UltralightGlfwOpenGLContext context;
    private long windowHandle;
    private UltralightView view;

    /**
     * Constructs a new instance of UltralightGlfwOpenGLWindow.
     *
     * @param context      the context to schedule execution of this window on
     * @param width        the initial width of this window
     * @param height       the initial height of this window
     * @param title        the initial title of this window
     * @param sharedWindow the GLFW window handle of the parent window. Expects 0 if no parent is given. OpenGL stats will be shared
     */
    private UltralightGlfwOpenGLWindow(UltralightGlfwOpenGLContext context, int width, int height, String title, long sharedWindow) {
        this.context = context;
        initialize(width, height, title, sharedWindow);
    }

    private void initialize(int width, int height, String title, long sharedWindow) {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        this.windowHandle = GLFW.glfwCreateWindow(width, height, title, MemoryUtil.NULL, sharedWindow);
    }

    private UltralightView createView() {
        return UltralightRendererInstanceHolder.getRenderer().createView(this.getWidth(), this.getHeight(), new UltralightViewConfig()
                .isAccelerated(true)
                .isTransparent(true));
    }

    /**
     * View will be created lazily on this call
     *
     * @return the {@link UltralightView} of this window
     * @see UltralightGlfwOpenGLWindow#createView()
     */
    public UltralightView getView() {
        if (this.view == null) {
            this.view = this.createView();
        }
        return this.view;
    }

    /**
     * @return the glfw window handle of this window
     */
    public long getWindowHandle() {
        return windowHandle;
    }


    /**
     * Renders the web content of this {@link com.labymedia.ultralight.UltralightView} to an OpenGL texture.
     *
     * @return this
     * @see UltralightOpenGLGPUDriver#renderTexture(UltralightGlfwOpenGLWindow)
     * @see UltralightOpenGLGPUDriver#initialize(UltralightGlfwOpenGLContext)
     * @see UltralightPlatform#setGPUDriver(UltralightGPUDriverNative)
     * @see UltralightPlatform#setGPUDriver(UltralightGPUDriver)
     * @see UltralightPlatform#setGPUDriverPointer(long)
     */
    public UltralightGlfwOpenGLWindow updateWebContent() {
        this.context.updateWebContent(this);
        return this;
    }

    /**
     * @return the associated context on which all executions of this window are scheduled
     */
    public UltralightGlfwOpenGLContext getContext() {
        return context;
    }

    /**
     * Constructs a new instance of UltralightGlfwOpenGLWindow.
     *
     * @param context the context to schedule execution of this window on
     * @param width   the initial width of this window
     * @param height  the initial height of this window
     * @param title   the initial title of this window
     */
    public static UltralightGlfwOpenGLWindow create(UltralightGlfwOpenGLContext context, int width, int height, String title) {
        return new UltralightGlfwOpenGLWindow(context, width, height, title, MemoryUtil.NULL);
    }

    /**
     * Constructs a new instance of UltralightGlfwOpenGLWindow.
     *
     * @param context      the context to schedule execution of this window on
     * @param width        the initial width of this window
     * @param height       the initial height of this window
     * @param title        the initial title of this window
     * @param parentWindow the GLFW window handle of the parent window. Expects 0 if no parent is given. OpenGL stats will be shared
     */
    public static UltralightGlfwOpenGLWindow create(UltralightGlfwOpenGLContext context, int width, int height, String title, long parentWindow) {
        return new UltralightGlfwOpenGLWindow(context, width, height, title, parentWindow);
    }

    /**
     * Schedule a task to run on the Ultralight thread.
     *
     * @param runnable the task to execute
     * @return a future that completes when the task is finished
     */
    public Future<?> post(Runnable runnable) {
        return getContext().post(runnable);
    }

    /**
     * Schedule a task to run on the Ultralight thread.
     *
     * @param callable the task to execute
     * @return a future that completes when the task is finished and returns the result of callable
     */
    public <R> Future<R> post(Callable<R> callable) {
        return getContext().post(callable);
    }

    /**
     * Schedule a task to run on the Ultralight thread and wait synchronously on its end of execution.
     *
     * @param callable the task to execute
     * @return The result of callable
     */
    public <R> R postAndWait(Callable<R> callable) {
        return getContext().postAndWait(callable);
    }

    /**
     * Schedule a task to run on the Ultralight thread and wait synchronously on its end of execution.
     *
     * @param runnable the task to execute
     * @return this
     */
    public UltralightGlfwOpenGLWindow postAndWait(Runnable runnable) {
        getContext().postAndWait(runnable);
        return this;
    }

    /**
     * Schedule a task to run on the Ultralight thread and wait synchronously on its end of execution.
     *
     * @param runnable the task to execute
     * @return this
     */
    public UltralightGlfwOpenGLWindow postAndWait(Consumer<UltralightGlfwOpenGLWindow> runnable) {
        getContext().postAndWait(() -> runnable.accept(this));
        return this;
    }

    /**
     * Makes this glfw window visible
     *
     * @return this
     */
    public UltralightGlfwOpenGLWindow show() {
        this.context.postAndWait(() -> glfwShowWindow(this.windowHandle));
        return this;
    }

    /**
     * Calls {@link GLFW#glfwMakeContextCurrent(long)} with this glfw window.
     *
     * @return this
     */
    public UltralightGlfwOpenGLWindow makeContext() {
        this.context.postAndWait(() -> glfwMakeContextCurrent(this.windowHandle));
        return this;
    }

    /**
     * Calls {@link GLFW#glfwSwapBuffers(long)} with this glfw window.
     *
     * @return this
     */
    public UltralightGlfwOpenGLWindow swapBuffers() {
        this.context.postAndWait(() -> glfwSwapBuffers(this.windowHandle));
        return this;
    }

    /**
     * @return {@link GLFW#glfwWindowShouldClose}
     */
    public boolean shouldClose() {
        return glfwWindowShouldClose(this.windowHandle);
    }

    /**
     * Destroys this window
     *
     * @return this
     */
    public UltralightGlfwOpenGLWindow close() {
        this.context.postAndWait(() -> glfwDestroyWindow(this.windowHandle));
        return this;
    }

    /**
     * @return the current width of this window
     */
    public int getWidth() {
        return this.context.postAndWait(() -> {
            int[] width = {0};
            int[] height = {0};
            glfwGetWindowSize(this.windowHandle, width, height);
            return width[0];
        });
    }

    /**
     * @return the current height of this window
     */
    public int getHeight() {
        return this.context.postAndWait(() -> {
            int[] width = {0};
            int[] height = {0};
            glfwGetWindowSize(this.windowHandle, width, height);
            return height[0];
        });
    }

    /**
     * Loads a website to this {@link UltralightView}.
     *
     * @param url the url to load
     * @see UltralightView#loadURL(String)
     */
    public void loadUrl(String url) {
        this.getView().loadURL(url);
    }

    /**
     * Overrides the current {@link UltralightView}. Should only be called if {@link UltralightGlfwOpenGLWindow#createView()} was never called.
     * This method exists only because the only way to use Ultralights web inspector is by using an existing {@link UltralightView} provided by {@link UltralightView#inspector()}.
     *
     * @param view the view to set
     * @return this
     */
    public UltralightGlfwOpenGLWindow setView(UltralightView view) {
        this.view = view;
        return this;
    }

    /**
     * Bind the rendered OpenGL texture of a this window to the current OpenGL context.
     *
     * @return this
     * @see UltralightGlfwOpenGLContext#updateWebContent(UltralightGlfwOpenGLWindow)
     * @see UltralightOpenGLGPUDriver#bindTexture(UltralightGlfwOpenGLWindow)
     */
    public UltralightGlfwOpenGLWindow bindTexture() {
        this.context.bindTexture(this);
        return this;
    }
}
