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

import com.labymedia.ultralight.UltralightView;
import com.labymedia.ultralight.input.UltralightCursor;
import com.labymedia.ultralight.math.IntRect;
import com.labymedia.ultralight.plugin.view.MessageLevel;
import com.labymedia.ultralight.plugin.view.MessageSource;
import com.labymedia.ultralight.plugin.view.UltralightViewListener;

/**
 * Test view listener for the main view
 */
public class TestViewListener implements UltralightViewListener {
    private final TestCursorManager cursorManager;

    public TestViewListener(TestCursorManager cursorManager) {
        this.cursorManager = cursorManager;
    }

    @Override
    public void onChangeTitle(String title) {
        System.out.println("View title has changed: " + title);
    }

    @Override
    public void onChangeURL(String url) {
        System.out.println("View url has changed: " + url);
    }

    @Override
    public void onChangeTooltip(String tooltip) {
        // System.out.println("View tooltip has changed: " + tooltip);
    }

    @Override
    public void onChangeCursor(UltralightCursor cursor) {
        this.cursorManager.notifyCursorUpdated(cursor);
    }

    @Override
    public void onAddConsoleMessage(
            MessageSource source,
            MessageLevel level,
            String message,
            long lineNumber,
            long columnNumber,
            String sourceId
    ) {
        System.out.println(
                "View message: ["
                        + source.name() + "/" + level.name() + "] "
                        + sourceId + ":" + lineNumber + ":" + columnNumber + ": " + message);
    }

    @Override
    public UltralightView onCreateChildView(String openerUrl, String targetUrl, boolean isPopup, IntRect popupRect) {
        System.out.println("View wants child: ");
        System.out.println("\tFrom: " + openerUrl);
        System.out.println("\tTo: " + targetUrl);
        System.out.println("\tIs popup: " + isPopup);
        if(popupRect.isValid()) {
            System.out.println("\tTarget rect: (" +
                    popupRect.x() + "," + popupRect.y() + " -> " + popupRect.width() + "," + popupRect.height() + ")");
        } else {
            System.out.println("\tTarget rect: NONE");
        }
        System.out.println("Cancelling request, multi view not implemented");
        return null;
    }
}
