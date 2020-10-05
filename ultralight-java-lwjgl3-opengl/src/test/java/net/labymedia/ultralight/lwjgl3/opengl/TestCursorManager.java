/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package net.labymedia.ultralight.lwjgl3.opengl;

import net.labymedia.ultralight.input.UltralightCursor;

import static org.lwjgl.glfw.GLFW.*;

/**
 * Utility class for controlling GLFW cursors.
 */
public class TestCursorManager {
    private final long window;

    private final long iBeamCursor;
    private final long crosshairCursor;
    private final long handCursor;
    private final long hresizeCursor;
    private final long vresizeCursor;

    /**
     * Creates a new {@link TestCursorManager} for the given window.
     *
     * @param window The window to manage cursors on
     */
    public TestCursorManager(long window) {
        this.window = window;

        this.iBeamCursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
        this.crosshairCursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
        this.handCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
        this.hresizeCursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
        this.vresizeCursor = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    }

    /**
     * Signals this {@link TestCursorManager} that the cursor has been updated and needs to be updated on the GLFW side
     * too.
     *
     * @param cursor The new {@link UltralightCursor} to display
     */
    public void notifyCursorUpdated(UltralightCursor cursor) {
        switch(cursor) {
            case CROSS:
                glfwSetCursor(window, crosshairCursor);
                break;

            case HAND:
                glfwSetCursor(window, handCursor);
                break;

            case I_BEAM:
                glfwSetCursor(window, iBeamCursor);
                break;

            case EAST_WEST_RESIZE:
                glfwSetCursor(window, hresizeCursor);
                break;

            case NORTH_SOUTH_RESIZE:
                glfwSetCursor(window, vresizeCursor);
                break;

            default:
                // No matching GLFW cursor
                glfwSetCursor(window, 0);
                break;
        }
    }

    /**
     * Frees GLFW resources allocated by this {@link TestCursorManager}.
     */
    public void cleanup() {
        glfwDestroyCursor(vresizeCursor);
        glfwDestroyCursor(hresizeCursor);
        glfwDestroyCursor(handCursor);
        glfwDestroyCursor(crosshairCursor);
        glfwDestroyCursor(iBeamCursor);
    }
}
