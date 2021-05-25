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
import com.labymedia.ultralight.UltralightRenderer;
import com.labymedia.ultralight.plugin.render.UltralightGPUDriver;
import com.labymedia.ultralight.plugin.render.UltralightGPUDriverNative;
import org.lwjgl.system.MemoryUtil;

import java.util.Collection;
import java.util.HashSet;
import java.util.concurrent.*;
import java.util.function.Consumer;

import static org.lwjgl.opengl.GL.createCapabilities;

/**
 * Util class to manage a whole lwjgl OpenGL and Ultralight context with low effort in a separate thread.
 * Should only be instantiated once because Ultralight is only capable of using one context at a time.
 * This may or may not change in the future.
 * <p>
 * A UltralightGlfwOpenGLContext always handles a main window and can optionally handle more sub-windows which will share their OpenGL contexts.
 * Using this class might add some small overhead to your program but will allow for asynchronous rendering and javascript execution.
 */
public class UltralightGlfwOpenGLContext {

    private final UltralightThread thread;
    private final Collection<UltralightGlfwOpenGLWindow> subWindows;
    private final UltralightOpenGLGPUDriver driver;

    private UltralightPlatform platform;
    private UltralightGlfwOpenGLWindow mainWindow;

    /**
     * Instantiate a new UltralightGlfwOpenGLContext.
     *
     * @param mainWindowWidth  the initial width of the main window
     * @param mainWindowHeight the initial height of the main window
     * @param mainWindowTitle  the initial title of the main window
     * @param driver           the driver instance to use in this context
     * @param parentWindow     the GLFW window handle of the parent window. Expects 0 if no parent is given
     */
    private UltralightGlfwOpenGLContext(int mainWindowWidth, int mainWindowHeight, String mainWindowTitle, UltralightOpenGLGPUDriver driver, long parentWindow) {
        this.driver = driver;
        this.thread = new UltralightThread();
        this.subWindows = new HashSet<>();
        initialize(mainWindowWidth, mainWindowHeight, mainWindowTitle, parentWindow);
    }

    private void initialize(int mainWindowWidth, int mainWindowHeight, String mainWindowTitle, long parentWindow) {
        this.mainWindow = UltralightGlfwOpenGLWindow.create(this, mainWindowWidth, mainWindowHeight, mainWindowTitle, parentWindow);
        this.postAndWait(() -> {
            this.mainWindow.makeContext();
            createCapabilities();
            this.platform = UltralightPlatform.instance();
            this.driver.initialize(this);
        });
    }

    /**
     * Schedule a task to run on the Ultralight thread.
     *
     * @param runnable the task to execute
     * @return a future that completes when the task is finished
     */
    public Future<?> post(Runnable runnable) {
        return thread.post(runnable);
    }

    /**
     * Schedule a task to run on the Ultralight thread.
     *
     * @param callable the task to execute
     * @return a future that completes when the task is finished and returns the result of callable
     */
    public <R> Future<R> post(Callable<R> callable) {
        return thread.post(callable);
    }

    /**
     * Schedule a task to run on the Ultralight thread and wait synchronously on its end of execution.
     *
     * @param runnable the task to execute
     * @return this
     */
    public UltralightGlfwOpenGLContext postAndWait(Runnable runnable) {
        thread.postAndWait(runnable);
        return this;
    }

    /**
     * Schedule a task to run on the Ultralight thread and wait synchronously on its end of execution.
     *
     * @param callable the task to execute
     * @return The result of callable
     */
    public <R> R postAndWait(Callable<R> callable) {
        return thread.postAndWait(callable);
    }

    /**
     * Schedule a task to run on the Ultralight thread and wait synchronously on its end of execution.
     *
     * @param consumer the task to execute with this as a parameter
     * @return this
     */
    public UltralightGlfwOpenGLContext postAndWait(Consumer<UltralightGlfwOpenGLContext> consumer) {
        thread.postAndWait(() -> consumer.accept(this));
        return this;
    }

    /**
     * @return the main window of this context
     */
    public UltralightGlfwOpenGLWindow getMainWindow() {
        return mainWindow;
    }

    /**
     * @return all sub windows of this context
     */
    public Collection<UltralightGlfwOpenGLWindow> getSubWindows() {
        return subWindows;
    }

    /**
     * Creates a sub window and registers it on this context so it will be retrievable through {@link UltralightGlfwOpenGLContext#getSubWindows()}.
     *
     * @param width  the initial width of the sub window
     * @param height the initial height of the sub window
     * @param title  the initial title of the sub window
     * @return the created sub window
     */
    public synchronized UltralightGlfwOpenGLWindow createSubWindow(int width, int height, String title) {
        UltralightGlfwOpenGLWindow window = UltralightGlfwOpenGLWindow.create(this, width, height, title, this.getMainWindow().getParentWindow());
        this.subWindows.add(window);
        return window;
    }

    /**
     * Instantiate a new UltralightGlfwOpenGLContext.
     *
     * @param mainWindowWidth  the initial width of the main window
     * @param mainWindowHeight the initial height of the main window
     * @param mainWindowTitle  the initial title of the main window
     * @param driver           the driver instance to use in this context
     */
    public static UltralightGlfwOpenGLContext create(int mainWindowWidth, int mainWindowHeight, String mainWindowTitle, UltralightOpenGLGPUDriver driver) {
        return new UltralightGlfwOpenGLContext(mainWindowWidth, mainWindowHeight, mainWindowTitle, driver, MemoryUtil.NULL);
    }

    /**
     * Instantiate a new UltralightGlfwOpenGLContext.
     *
     * @param mainWindowWidth  the initial width of the main window
     * @param mainWindowHeight the initial height of the main window
     * @param mainWindowTitle  the initial title of the main window
     * @param driver           the driver instance to use in this context
     * @param parentWindow     the GLFW window handle of the parent window. Expects 0 if no parent is given
     */
    public static UltralightGlfwOpenGLContext create(int mainWindowWidth, int mainWindowHeight, String mainWindowTitle, UltralightOpenGLGPUDriver driver, long parentWindow) {
        return new UltralightGlfwOpenGLContext(mainWindowWidth, mainWindowHeight, mainWindowTitle, driver, parentWindow);
    }

    /**
     * @return the global singleton instance of {@link UltralightPlatform}
     */
    public UltralightPlatform getPlatform() {
        return this.platform;
    }

    /**
     * Schedules javascript update on all present {@link com.labymedia.ultralight.UltralightView}s.
     *
     * @see UltralightRenderer#update()
     */
    public UltralightGlfwOpenGLContext updateJavaScript() {
        this.postAndWait(() -> UltralightRendererInstanceHolder.getRenderer().update());
        return this;
    }

    /**
     * Renders the web content of a given {@link com.labymedia.ultralight.UltralightView} to an OpenGL texture.
     *
     * @param window the window to render
     * @return this
     * @see UltralightOpenGLGPUDriver#renderTexture(UltralightGlfwOpenGLWindow)
     * @see UltralightOpenGLGPUDriver#initialize(UltralightGlfwOpenGLContext)
     * @see UltralightPlatform#setGPUDriver(UltralightGPUDriverNative)
     * @see UltralightPlatform#setGPUDriver(UltralightGPUDriver)
     * @see UltralightPlatform#setGPUDriverPointer(long)
     */
    public UltralightGlfwOpenGLContext updateWebContent(UltralightGlfwOpenGLWindow window) {
        this.postAndWait(() -> this.driver.renderTexture(window));
        return this;
    }

    /**
     * Bind the rendered OpenGL texture of a given window to the current OpenGL context.
     *
     * @param window the window to retrieve the texture from
     * @return this
     * @see UltralightGlfwOpenGLContext#updateWebContent(UltralightGlfwOpenGLWindow)
     * @see UltralightOpenGLGPUDriver#bindTexture(UltralightGlfwOpenGLWindow)
     */
    public UltralightGlfwOpenGLContext bindTexture(UltralightGlfwOpenGLWindow window) {
        this.driver.bindTexture(window);
        return this;
    }

    /**
     * @return the instance of the specified Ultralight GPU driver
     */
    public UltralightOpenGLGPUDriver getDriver() {
        return this.driver;
    }

    /**
     * @return true if {@link Thread#currentThread()} is equals to the ultralight thread or false
     */
    public boolean isOnUltralightThread() {
        return this.thread.isOnUltralightThread();
    }

    private static class UltralightThread extends Thread {
        private final ScheduledExecutorService executorService;

        private Runnable delegate;

        public UltralightThread() {
            this.executorService = Executors
                    .newSingleThreadScheduledExecutor(new UltralightThreadFactory(this));

            setName("Ultralight Thread");
        }


        /**
         * Posts a runnable on the thread, or if on the thread already, executes it immediately.
         *
         * @param runnable The runnable to post
         * @return A future wrapping the execution of the runnable
         */
        public Future<?> post(Runnable runnable) {
            if (isOnUltralightThread()) {
                // Run immediately
                FutureTask<?> task = new FutureTask<Void>(runnable, null);
                task.run();
                return task;
            }

            return executorService.submit(runnable);
        }

        /**
         * Posts a callable on the thread, or if on the thread already, executes it immediately.
         *
         * @param callable The callable to post
         * @param <R>      The return type of a callable
         * @return A future wrapping the execution of the callable
         */
        public <R> Future<R> post(Callable<R> callable) {
            if (isOnUltralightThread()) {
                // Run immediately
                FutureTask<R> task = new FutureTask<>(callable);
                task.run();
                return task;
            }

            return executorService.submit(callable);
        }

        /**
         * Posts a callable on the thread and waits for it to complete.
         *
         * @param callable The callable to post
         * @param <R>      The return type of the callable
         * @return The return value of the callable
         */
        public <R> R postAndWait(Callable<R> callable) {
            try {
                return post(callable).get();
            } catch (InterruptedException | ExecutionException e) {
                throw new IllegalStateException("Exception while executing task on web thread", e);
            }
        }

        /**
         * Posts a runnable on the thread and waits for it to complete.
         *
         * @param runnable The runnable to post
         */
        public void postAndWait(Runnable runnable) {
            try {
                post(runnable).get();
            } catch (InterruptedException | ExecutionException e) {
                throw new IllegalStateException("Exception while executing task on web thread", e);
            }
        }

        /**
         * Tests whether the current thread is the Ultralight thread.
         *
         * @return {@code true} if the current thread is the Ultralight thread, {@code false} otherwise
         */
        public boolean isOnUltralightThread() {
            return Thread.currentThread().equals(this);
        }

        @Override
        public void run() {
            this.delegate.run();
        }

        public void setDelegate(Runnable delegate) {
            this.delegate = delegate;
        }
    }

    /**
     * One-time use factory for the Ultralight executor service.
     */
    private static class UltralightThreadFactory implements ThreadFactory {

        private UltralightThread thread;

        /**
         * Constructs a new factory for the Ultralight thread.
         */
        private UltralightThreadFactory(UltralightThread thread) {
            this.thread = thread;
        }

        @SuppressWarnings("NullableProblems")
        @Override
        public Thread newThread(Runnable r) {
            if (thread == null) {
                throw new IllegalStateException("Tried to schedule a runnable after Ultralight died");
            }

            // Copy the reference and null out the original one so that the thread can't be created twice
            UltralightThread created = thread;
            thread = null;

            // Tell the thread what to do after initialization
            created.setDelegate(r);
            return created;
        }
    }
}
