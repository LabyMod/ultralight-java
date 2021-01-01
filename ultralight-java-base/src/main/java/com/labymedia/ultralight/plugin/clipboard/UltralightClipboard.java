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

package com.labymedia.ultralight.plugin.clipboard;

/**
 * Clipboard interface.
 * <p>
 * This is used for reading and writing data to the platform Clipboard.
 * <p>
 * This is intended to be implemented by users and defined before creating the Renderer. @see Platform::set_clipboard.
 */
public interface UltralightClipboard {
    /**
     * Clear the clipboard.
     */
    void clear();

    /**
     * Read plain text from the clipboard
     *
     * @return The content of the clipboard as plain text
     */
    String readPlainText();

    /**
     * Write plain text to the clipboard.
     *
     * @param text The plain text to write to the clipboard
     */
    void writePlainText(String text);
}
