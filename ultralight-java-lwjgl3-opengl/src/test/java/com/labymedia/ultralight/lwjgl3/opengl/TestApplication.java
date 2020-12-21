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

package com.labymedia.ultralight.lwjgl3.opengl;

import org.lwjgl.glfw.GLFW;
import org.lwjgl.glfw.GLFWVidMode;
import org.lwjgl.opengl.GL;
import org.lwjgl.system.Callback;
import org.lwjgl.system.MemoryStack;
import org.lwjgl.system.MemoryUtil;

import java.nio.FloatBuffer;
import java.nio.IntBuffer;
import java.util.function.BiFunction;
import java.util.function.Function;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL20.*;

/**
 * Test application class using GLFW windows.
 */
public class TestApplication {
    private final long window;
    private final TestInputAdapter inputAdapter;
    private final TestCursorManager cursorManager;
    private final WebController webController;

    public TestApplication() {
        // Set up an error callback
        setCallback(GLFW::glfwSetErrorCallback, this::onGLFWError);

        // Prepare GLFW for use
        if(!glfwInit()) {
            throw new IllegalStateException("Failed to initialize GLFW");
        }

        // Create a GLFW window
        window = glfwCreateWindow(640, 480, "Ultralight GLFW", MemoryUtil.NULL, MemoryUtil.NULL);
        if(window == MemoryUtil.NULL) {
            // Window creation failed
            stop();
            throw new IllegalStateException("Failed to create a GLFW window");
        }

        // Make sure to update the framebuffer size when resizing
        setCallback(GLFW::glfwSetFramebufferSizeCallback, this::updateSize);

        this.cursorManager = new TestCursorManager(window);
        this.webController = new WebController(cursorManager);

        this.inputAdapter = webController.getInputAdapter();

        setCallback(GLFW::glfwSetWindowContentScaleCallback, inputAdapter::windowContentScaleCallback);
        setCallback(GLFW::glfwSetKeyCallback, inputAdapter::keyCallback);
        setCallback(GLFW::glfwSetCharCallback, inputAdapter::charCallback);
        setCallback(GLFW::glfwSetCursorPosCallback, inputAdapter::cursorPosCallback);
        setCallback(GLFW::glfwSetMouseButtonCallback, inputAdapter::mouseButtonCallback);
        setCallback(GLFW::glfwSetScrollCallback, inputAdapter::scrollCallback);
        setCallback(GLFW::glfwSetWindowFocusCallback, inputAdapter::focusCallback);
    }

    /**
     * Centers the window on screen
     */
    public void centerWindow() {
        // Create a memory stack so we don't have to worry about free's
        try(MemoryStack stack = MemoryStack.stackPush()) {
            // Retrieve current monitor of the window
            long monitor = glfwGetWindowMonitor(window);
            if(monitor == MemoryUtil.NULL) {
                // The window is not on any monitor, get the primary one
                monitor = glfwGetPrimaryMonitor();
            }

            // If there is no monitor, we can't center the window
            if(monitor == MemoryUtil.NULL) {
                return;
            }

            // Retrieve the video mode of the monitor
            GLFWVidMode videoMode = glfwGetVideoMode(monitor);
            if(videoMode == null) {
                // The monitor has no video mode?
                return;
            }

            // Get a buffer with 2 ints to store the position of the monitor in
            IntBuffer monitorPosBuffer = stack.callocInt(2);

            // Store the x position in slot 0 and the y position in slot 1 of the buffer
            glfwGetMonitorPos(monitor,
                    (IntBuffer) monitorPosBuffer.slice().position(0), (IntBuffer) monitorPosBuffer.slice().position(1));

            // Extract the x and y positions from the buffer
            int monitorX = monitorPosBuffer.get(0);
            int monitorY = monitorPosBuffer.get(1);

            // Get a buffer with 2 ints to store the size of the window in
            IntBuffer windowSizeBuffer = stack.callocInt(2);

            // Store the window width in slot 0 and the window height in slot 1 of the buffer
            glfwGetWindowSize(window,
                    (IntBuffer) windowSizeBuffer.slice().position(0), (IntBuffer) windowSizeBuffer.slice().position(1));

            // Extract the window width and window height from the buffer
            int windowWidth = windowSizeBuffer.get(0);
            int windowHeight = windowSizeBuffer.get(1);

            // Center the window on the monitor
            glfwSetWindowPos(
                    window,
                    monitorX + (videoMode.width() - windowWidth) / 2,
                    monitorY + (videoMode.height() - windowHeight) / 2
            );
        }
    }

    /**
     * Stops the application and destroys allocated resources
     */
    public void stop() {
        if(window != MemoryUtil.NULL) {
            // Delete the window if it is not null
            glfwDestroyWindow(window);
        }

        // Clean up native resources
        cursorManager.cleanup();

        // Shut down GLFW
        glfwTerminate();

        // Terminate now, will prevent warnings on shutdown
        System.exit(0);
    }

    /**
     * Enters the run loop of the application.
     */
    public void run() {
        // Make the window's OpenGL context the current one
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        // Initialize OpenGL capabilities
        GL.createCapabilities();

        // Manually update focus for the first time
        inputAdapter.focusCallback(window, glfwGetWindowAttrib(window, GLFW_FOCUSED) != 0);

        try(MemoryStack stack = MemoryStack.stackPush()) {
            // Update window size for the first time
            IntBuffer sizeBuffer = stack.callocInt(2);

            // Retrieve the size into the int buffer
            glfwGetWindowSize(window,
                    (IntBuffer) sizeBuffer.slice().position(0), (IntBuffer) sizeBuffer.slice().position(1));

            // Update the size
            updateSize(window, sizeBuffer.get(0), sizeBuffer.get(1));

            /*
             * Following snippet disabled due to GLFW bug, glfwGetWindowContentScale returns invalid values!
             *
             * On a test system
             */
            // Update scale for the first time
            // FloatBuffer scaleBuffer = stack.callocFloat(2);

            // Retrieve the scale into the float buffer
            // glfwGetWindowContentScale(window,
            //        (FloatBuffer) scaleBuffer.slice().position(0), (FloatBuffer) scaleBuffer.slice().position(1));

            // Retrieve framebuffer size for scale calculation
            IntBuffer framebufferSizeBuffer = stack.callocInt(2);

            // Retrieve the size into the int buffer
            glfwGetFramebufferSize(window,
                (IntBuffer) framebufferSizeBuffer.slice().position(0), (IntBuffer) sizeBuffer.slice().position(1));

            // Calculate scale
            float xScale = ((float) (framebufferSizeBuffer.get(0))) / ((float) (sizeBuffer.get(0)));
            float yScale = ((float) (framebufferSizeBuffer.get(1))) / ((float) (sizeBuffer.get(1)));

            // Fix up scale in case it gets corrupted... somehow
            if(xScale == 0.0f) {
                xScale = 1.0f;
            }

            if(yScale == 0.0f) {
                yScale = 1.0f;
            }

            // Update the scale
            inputAdapter.windowContentScaleCallback(window, xScale, yScale);
        }

        glEnable(GL_MULTISAMPLE);

        // Set opaque black as the clear color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        webController.loadURL("file:///test.html");

        double lastTime = glfwGetTime();
        int frameCount = 0;

        OpenGLDrawer drawer = new OpenGLDrawer();

        // Keep running until a window close is requested
        while(!glfwWindowShouldClose(window)) {
            // Poll events to keep the window responsive
            glfwPollEvents();

            webController.update();

            // Clear the color and depth buffer and then draw
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            drawer.draw();
            webController.render();

            double currentTime = glfwGetTime();
            frameCount++;
            if(currentTime - lastTime >= 1.0) {
                double msPerFrame = 1000.0 / ((double) frameCount);
                glfwSetWindowTitle(window, "Ultralight GLFW (" + msPerFrame + " | " + frameCount + ")");
                frameCount = 0;
                lastTime += 1.0;
            }

            // Redraw the window
            glfwSwapBuffers(window);
        }

        stop();
    }

    /**
     * Adjusts the viewport size to the given size.
     *
     * @param window The window the viewport has changed on
     * @param width  The new width of the viewport
     * @param height The new height of the viewport
     */
    private void updateSize(long window, int width, int height) {
        System.out.println("Resizing to " + width + "x" + height);
        glViewport(0, 0, width, height);
        webController.resize(width, height);
    }

    /**
     * Callback notified when an error occurs in GLFW.
     *
     * @param error   The error code
     * @param message A human readable error message
     */
    private void onGLFWError(int error, long message) {
        String strMessage = MemoryUtil.memUTF8(message);
        System.err.println("[GLFW] Error(" + error + "): " + strMessage);
    }

    /**
     * Sets a GLFW callback and frees the old callback if it exists.
     *
     * @param setter   The function to use for setting the new callback
     * @param newValue The new callback
     * @param <T>      The type of the new callback
     * @param <C>      The type of the old callback
     */
    private <T, C extends Callback> void setCallback(Function<T, C> setter, T newValue) {
        C oldValue = setter.apply(newValue);
        if(oldValue != null) {
            oldValue.free();
        }
    }

    /**
     * Sets a GLFW callback and frees the old callback if it exists.
     *
     * @param setter   The function to use for setting the new callback
     * @param newValue The new callback
     * @param <T>      The type of the new callback
     * @param <C>      The type of the old callback
     */
    private <T, C extends Callback> void setCallback(BiFunction<Long, T, C> setter, T newValue) {
        C oldValue = setter.apply(window, newValue);
        if(oldValue != null) {
            oldValue.free();
        }
    }
}
