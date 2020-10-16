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

package net.labymedia.ultralight.lwjgl3.opengl;

import net.labymedia.ultralight.UltralightView;
import net.labymedia.ultralight.input.*;
import org.lwjgl.system.MemoryStack;

import java.nio.DoubleBuffer;

import static org.lwjgl.glfw.GLFW.*;

/**
 * Class for converting GLFW events into Ultralight events.
 */
public class TestInputAdapter {
    private final UltralightView view;

    private float xScale;
    private float yScale;

    /**
     * Constructs a new {@link TestInputAdapter} for the specified view.
     *
     * @param view The view to send events to
     */
    public TestInputAdapter(UltralightView view) {
        this.view = view;
    }

    /**
     * Called by GLFW then the content scale (DPI ratio) changes.
     *
     * @param window The window that caused the event
     * @param xScale The new x scale
     * @param yScale The new y scale
     */
    public void windowContentScaleCallback(long window, float xScale, float yScale) {
        this.xScale = xScale;
        this.yScale = yScale;
    }

    /**
     * Called by GLFW when a key is pressed.
     *
     * @param window   The window that caused the event
     * @param key      The GLFW keycode
     * @param scancode The keyboard scancode
     * @param action   The GLFW action
     * @param mods     The key modifiers
     */
    public void keyCallback(long window, int key, int scancode, int action, int mods) {
        UltralightKey translatedKey = glfwToUltralightKey(key);

        // Build the event
        UltralightKeyEvent event = new UltralightKeyEvent()
                .type(action == GLFW_PRESS || action == GLFW_REPEAT ?
                        UltralightKeyEventType.RAW_DOWN : UltralightKeyEventType.UP)
                .virtualKeyCode(translatedKey)
                .nativeKeyCode(scancode)
                .keyIdentifier(UltralightKeyEvent.getKeyIdentifierFromVirtualKeyCode(translatedKey))
                .modifiers(glfwToUltralightModifiers(mods));

        // Send the event
        view.fireKeyEvent(event);

        if((action == GLFW_PRESS || action == GLFW_REPEAT) && (key == GLFW_KEY_ENTER || key == GLFW_KEY_TAB)) {
            // These keys need to be translated specially
            String text = key == GLFW_KEY_ENTER ? "\r" : "\t";
            UltralightKeyEvent extraEvent = new UltralightKeyEvent()
                    .type(UltralightKeyEventType.CHAR)
                    .text(text)
                    .unmodifiedText(text);

            // Fire the event
            view.fireKeyEvent(extraEvent);
        }
    }

    /**
     * Called by GLFW when a char is input.
     *
     * @param window    The window that caused the event
     * @param codepoint The unicode char that has been input
     */
    public void charCallback(long window, int codepoint) {
        // Convert the unicode code point to a UTF-16 string
        String text = new String(Character.toChars(codepoint));

        // Create the event
        UltralightKeyEvent event = new UltralightKeyEvent()
                .type(UltralightKeyEventType.CHAR)
                .text(text)
                .unmodifiedText(text);

        // Fire the event
        view.fireKeyEvent(event);
    }

    /**
     * Called by GLFW when the mouse moves.
     *
     * @param window The window that caused the event
     * @param x      The new x position of the cursor
     * @param y      The new y position of the cursor
     */
    public void cursorPosCallback(long window, double x, double y) {
        // Create the event
        UltralightMouseEvent event = new UltralightMouseEvent()
                .x((int) (x * xScale))
                .y((int) (y * yScale))
                .type(UltralightMouseEventType.MOVED);

        // Translate the mouse state to the event
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            event.button(UltralightMouseEventButton.LEFT);
        } else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
            event.button(UltralightMouseEventButton.MIDDLE);
        } else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            event.button(UltralightMouseEventButton.RIGHT);
        }

        // Fire the event
        view.fireMouseEvent(event);
    }

    /**
     * Called by GLFW when a mouse button changes its state.
     *
     * @param window The window that caused the event
     * @param button the button that changed its state
     * @param action The new state of the button
     * @param mods   The mouse modifiers
     */
    public void mouseButtonCallback(long window, int button, int action, int mods) {
        double x;
        double y;

        // Use a memory stack so we don't have to worry about freeing allocations
        try(MemoryStack stack = MemoryStack.stackPush()) {
            DoubleBuffer buffer = stack.callocDouble(2);

            // Retrieve the current cursor pos
            glfwGetCursorPos(window, (DoubleBuffer) buffer.slice().position(0), (DoubleBuffer) buffer.slice().position(1));

            // Extract the x and y position
            x = buffer.get(0);
            y = buffer.get(1);
        }

        // Create the event
        UltralightMouseEvent event = new UltralightMouseEvent()
                .x((int) (x * xScale))
                .y((int) (y * yScale))
                .type(action == GLFW_PRESS ? UltralightMouseEventType.DOWN : UltralightMouseEventType.UP);

        // Sort out the button
        switch(button) {
            case GLFW_MOUSE_BUTTON_LEFT:
                event.button(UltralightMouseEventButton.LEFT);
                break;

            case GLFW_MOUSE_BUTTON_MIDDLE:
                event.button(UltralightMouseEventButton.MIDDLE);
                break;

            case GLFW_MOUSE_BUTTON_RIGHT:
                event.button(UltralightMouseEventButton.RIGHT);
                break;
        }

        // Fire the event
        view.fireMouseEvent(event);
    }

    /**
     * Called by GLFW when the user scrolls within the window.
     *
     * @param window The window that caused the event
     * @param xDelta The x scroll delta
     * @param yDelta The y scroll delta
     */
    public void scrollCallback(long window, double xDelta, double yDelta) {
        // Create the event
        UltralightScrollEvent event = new UltralightScrollEvent()
                .deltaX((int) xDelta * 32)
                .deltaY((int) yDelta * 32)
                .type(UltralightScrollEventType.BY_PIXEL);

        // Fire the event
        view.fireScrollEvent(event);
    }

    /**
     * Called by GLFW when the window gains or looses focus.
     *
     * @param window The window that caused the event
     * @param focus  Whether the window gained focus
     */
    public void focusCallback(long window, boolean focus) {
        if(focus) {
            view.focus();
        } else {
            view.unfocus();
        }
    }

    /**
     * Translates GLFW key modifiers to Ultralight key modifiers.
     *
     * @param modifiers The GLFW key modifiers to translate
     * @return The translated Ultralight key modifiers
     */
    private int glfwToUltralightModifiers(int modifiers) {
        int ultralightModifiers = 0;

        if((modifiers & GLFW_MOD_ALT) != 0) {
            ultralightModifiers |= UltralightInputModifier.ALT_KEY;
        }

        if((modifiers & GLFW_MOD_CONTROL) != 0) {
            ultralightModifiers |= UltralightInputModifier.CTRL_KEY;
        }

        if((modifiers & GLFW_MOD_SUPER) != 0) {
            ultralightModifiers |= UltralightInputModifier.META_KEY;
        }

        if((modifiers & GLFW_MOD_SHIFT) != 0) {
            ultralightModifiers |= UltralightInputModifier.SHIFT_KEY;
        }

        return ultralightModifiers;
    }

    /**
     * Translates a GLFW key code to an {@link UltralightKey}.
     *
     * @param key The GLFW key code to translate
     * @return The translated Ultralight key, or {@link UltralightKey#UNKNOWN}, if the key could not be translated
     */
    private UltralightKey glfwToUltralightKey(int key) {
        switch(key) {
            case GLFW_KEY_SPACE:
                return UltralightKey.SPACE;
            case GLFW_KEY_APOSTROPHE:
                return UltralightKey.OEM_7;
            case GLFW_KEY_COMMA:
                return UltralightKey.OEM_COMMA;
            case GLFW_KEY_MINUS:
                return UltralightKey.OEM_MINUS;
            case GLFW_KEY_PERIOD:
                return UltralightKey.OEM_PERIOD;
            case GLFW_KEY_SLASH:
                return UltralightKey.OEM_2;
            case GLFW_KEY_0:
                return UltralightKey.NUM_0;
            case GLFW_KEY_1:
                return UltralightKey.NUM_1;
            case GLFW_KEY_2:
                return UltralightKey.NUM_2;
            case GLFW_KEY_3:
                return UltralightKey.NUM_3;
            case GLFW_KEY_4:
                return UltralightKey.NUM_4;
            case GLFW_KEY_5:
                return UltralightKey.NUM_5;
            case GLFW_KEY_6:
                return UltralightKey.NUM_6;
            case GLFW_KEY_7:
                return UltralightKey.NUM_7;
            case GLFW_KEY_8:
                return UltralightKey.NUM_8;
            case GLFW_KEY_9:
                return UltralightKey.NUM_9;
            case GLFW_KEY_SEMICOLON:
                return UltralightKey.OEM_1;
            case GLFW_KEY_EQUAL:
            case GLFW_KEY_KP_EQUAL:
                return UltralightKey.OEM_PLUS;
            case GLFW_KEY_A:
                return UltralightKey.A;
            case GLFW_KEY_B:
                return UltralightKey.B;
            case GLFW_KEY_C:
                return UltralightKey.C;
            case GLFW_KEY_D:
                return UltralightKey.D;
            case GLFW_KEY_E:
                return UltralightKey.E;
            case GLFW_KEY_F:
                return UltralightKey.F;
            case GLFW_KEY_G:
                return UltralightKey.G;
            case GLFW_KEY_H:
                return UltralightKey.H;
            case GLFW_KEY_I:
                return UltralightKey.I;
            case GLFW_KEY_J:
                return UltralightKey.J;
            case GLFW_KEY_K:
                return UltralightKey.K;
            case GLFW_KEY_L:
                return UltralightKey.L;
            case GLFW_KEY_M:
                return UltralightKey.M;
            case GLFW_KEY_N:
                return UltralightKey.N;
            case GLFW_KEY_O:
                return UltralightKey.O;
            case GLFW_KEY_P:
                return UltralightKey.P;
            case GLFW_KEY_Q:
                return UltralightKey.Q;
            case GLFW_KEY_R:
                return UltralightKey.R;
            case GLFW_KEY_S:
                return UltralightKey.S;
            case GLFW_KEY_T:
                return UltralightKey.T;
            case GLFW_KEY_U:
                return UltralightKey.U;
            case GLFW_KEY_V:
                return UltralightKey.V;
            case GLFW_KEY_W:
                return UltralightKey.W;
            case GLFW_KEY_X:
                return UltralightKey.X;
            case GLFW_KEY_Y:
                return UltralightKey.Y;
            case GLFW_KEY_Z:
                return UltralightKey.Z;
            case GLFW_KEY_LEFT_BRACKET:
                return UltralightKey.OEM_4;
            case GLFW_KEY_BACKSLASH:
                return UltralightKey.OEM_5;
            case GLFW_KEY_RIGHT_BRACKET:
                return UltralightKey.OEM_6;
            case GLFW_KEY_GRAVE_ACCENT:
                return UltralightKey.OEM_3;
            case GLFW_KEY_ESCAPE:
                return UltralightKey.ESCAPE;
            case GLFW_KEY_ENTER:
            case GLFW_KEY_KP_ENTER:
                return UltralightKey.RETURN;
            case GLFW_KEY_TAB:
                return UltralightKey.TAB;
            case GLFW_KEY_BACKSPACE:
                return UltralightKey.BACK;
            case GLFW_KEY_INSERT:
                return UltralightKey.INSERT;
            case GLFW_KEY_DELETE:
                return UltralightKey.DELETE;
            case GLFW_KEY_RIGHT:
                return UltralightKey.RIGHT;
            case GLFW_KEY_LEFT:
                return UltralightKey.LEFT;
            case GLFW_KEY_DOWN:
                return UltralightKey.DOWN;
            case GLFW_KEY_UP:
                return UltralightKey.UP;
            case GLFW_KEY_PAGE_UP:
                return UltralightKey.PRIOR;
            case GLFW_KEY_PAGE_DOWN:
                return UltralightKey.NEXT;
            case GLFW_KEY_HOME:
                return UltralightKey.HOME;
            case GLFW_KEY_END:
                return UltralightKey.END;
            case GLFW_KEY_CAPS_LOCK:
                return UltralightKey.CAPITAL;
            case GLFW_KEY_SCROLL_LOCK:
                return UltralightKey.SCROLL;
            case GLFW_KEY_NUM_LOCK:
                return UltralightKey.NUMLOCK;
            case GLFW_KEY_PRINT_SCREEN:
                return UltralightKey.SNAPSHOT;
            case GLFW_KEY_PAUSE:
                return UltralightKey.PAUSE;
            case GLFW_KEY_F1:
                return UltralightKey.F1;
            case GLFW_KEY_F2:
                return UltralightKey.F2;
            case GLFW_KEY_F3:
                return UltralightKey.F3;
            case GLFW_KEY_F4:
                return UltralightKey.F4;
            case GLFW_KEY_F5:
                return UltralightKey.F5;
            case GLFW_KEY_F6:
                return UltralightKey.F6;
            case GLFW_KEY_F7:
                return UltralightKey.F7;
            case GLFW_KEY_F8:
                return UltralightKey.F8;
            case GLFW_KEY_F9:
                return UltralightKey.F9;
            case GLFW_KEY_F10:
                return UltralightKey.F10;
            case GLFW_KEY_F11:
                return UltralightKey.F11;
            case GLFW_KEY_F12:
                return UltralightKey.F12;
            case GLFW_KEY_F13:
                return UltralightKey.F13;
            case GLFW_KEY_F14:
                return UltralightKey.F14;
            case GLFW_KEY_F15:
                return UltralightKey.F15;
            case GLFW_KEY_F16:
                return UltralightKey.F16;
            case GLFW_KEY_F17:
                return UltralightKey.F17;
            case GLFW_KEY_F18:
                return UltralightKey.F18;
            case GLFW_KEY_F19:
                return UltralightKey.F19;
            case GLFW_KEY_F20:
                return UltralightKey.F20;
            case GLFW_KEY_F21:
                return UltralightKey.F21;
            case GLFW_KEY_F22:
                return UltralightKey.F22;
            case GLFW_KEY_F23:
                return UltralightKey.F23;
            case GLFW_KEY_F24:
                return UltralightKey.F24;
            case GLFW_KEY_KP_0:
                return UltralightKey.NUMPAD0;
            case GLFW_KEY_KP_1:
                return UltralightKey.NUMPAD1;
            case GLFW_KEY_KP_2:
                return UltralightKey.NUMPAD2;
            case GLFW_KEY_KP_3:
                return UltralightKey.NUMPAD3;
            case GLFW_KEY_KP_4:
                return UltralightKey.NUMPAD4;
            case GLFW_KEY_KP_5:
                return UltralightKey.NUMPAD5;
            case GLFW_KEY_KP_6:
                return UltralightKey.NUMPAD6;
            case GLFW_KEY_KP_7:
                return UltralightKey.NUMPAD7;
            case GLFW_KEY_KP_8:
                return UltralightKey.NUMPAD8;
            case GLFW_KEY_KP_9:
                return UltralightKey.NUMPAD9;
            case GLFW_KEY_KP_DECIMAL:
                return UltralightKey.DECIMAL;
            case GLFW_KEY_KP_DIVIDE:
                return UltralightKey.DIVIDE;
            case GLFW_KEY_KP_MULTIPLY:
                return UltralightKey.MULTIPLY;
            case GLFW_KEY_KP_SUBTRACT:
                return UltralightKey.SUBTRACT;
            case GLFW_KEY_KP_ADD:
                return UltralightKey.ADD;
            case GLFW_KEY_LEFT_SHIFT:
            case GLFW_KEY_RIGHT_SHIFT:
                return UltralightKey.SHIFT;
            case GLFW_KEY_LEFT_CONTROL:
            case GLFW_KEY_RIGHT_CONTROL:
                return UltralightKey.CONTROL;
            case GLFW_KEY_LEFT_ALT:
            case GLFW_KEY_RIGHT_ALT:
                return UltralightKey.MENU;
            case GLFW_KEY_LEFT_SUPER:
                return UltralightKey.LWIN;
            case GLFW_KEY_RIGHT_SUPER:
                return UltralightKey.RWIN;
            default:
                return UltralightKey.UNKNOWN;
        }
    }
}
