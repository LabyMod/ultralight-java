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

package com.labymedia.ultralight.plugin.view;

import com.labymedia.ultralight.UltralightRenderer;
import com.labymedia.ultralight.UltralightView;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.annotation.Unsigned;
import com.labymedia.ultralight.config.UltralightViewConfig;
import com.labymedia.ultralight.input.UltralightCursor;
import com.labymedia.ultralight.math.IntRect;

/**
 * Interface for View-related events
 */
public interface UltralightViewListener {
    /**
     * Called when the page title changes.
     *
     * @param title The new page title
     */
    void onChangeTitle(String title);

    /**
     * Called when the page URL changes.
     *
     * @param url The new page url
     */
    void onChangeURL(String url);

    /**
     * Called when the tooltip changes (usually as result of a mouse hover).
     *
     * @param tooltip The new page tooltip
     */
    void onChangeTooltip(String tooltip);

    /**
     * Called when the mouse cursor changes.
     *
     * @param cursor The new page cursor
     */
    void onChangeCursor(UltralightCursor cursor);

    /**
     * Called when a message is added to the console (useful for errors / debug).
     *
     * @param source       The source the message originated from
     * @param level        The severity of the message
     * @param message      The message itself
     * @param lineNumber   The line the message originated from
     * @param columnNumber The column the message originated from
     * @param sourceId     The id of the source
     */
    void onAddConsoleMessage(
            MessageSource source,
            MessageLevel level,
            String message,
            @NativeType("uint23_t") @Unsigned long lineNumber,
            @NativeType("uint32_t") @Unsigned long columnNumber,
            String sourceId
    );

    /**
     * Called when the page wants to create a new View.
     * <p>
     * This is usually the result of a user clicking a link with target="_blank" or by JavaScript calling
     * window.open(url).
     * <p>
     * To allow creation of these new Views, you should create a new View in this callback (eg, {@link
     * UltralightRenderer#createView(long, long, UltralightViewConfig)} )}), resize it to your container, and
     * return it. You are responsible for displaying the returned View.
     *
     * @param openerUrl The URL of the page that initiated this request
     * @param targetUrl The URL that the new View will navigate to
     * @param isPopup   Whether or not this was triggered by window.open()
     * @param popupRect Popups can optionally request certain dimensions and coordinates via window.open(). You can
     *                  choose to respect these or not by resizing/moving the View to this rect.
     * @return Returns a {@link UltralightView} to use to satisfy the the request (or return {@code null} if you want to
     * block the action).
     */
    UltralightView onCreateChildView(String openerUrl, String targetUrl, boolean isPopup, IntRect popupRect);
}
