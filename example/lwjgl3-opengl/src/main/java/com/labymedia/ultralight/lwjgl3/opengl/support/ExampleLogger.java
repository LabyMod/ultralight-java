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

import com.labymedia.ultralight.lwjgl3.opengl.listener.ExampleViewListener;
import com.labymedia.ultralight.plugin.logging.UltralightLogLevel;
import com.labymedia.ultralight.plugin.logging.UltralightLogger;
import com.labymedia.ultralight.plugin.view.MessageLevel;
import com.labymedia.ultralight.plugin.view.MessageSource;

/**
 * Example implementation of a logger
 */
public class ExampleLogger implements UltralightLogger {
    /**
     * This is called by Ultralight every time a message needs to be logged. Note that Ultralight messages may include
     * new lines, so if you want really pretty log output reformat the string accordingly.
     * <p>
     * This logger is <b>NOT</b> called for {@code console.log} messages, see {@link
     * ExampleViewListener#onAddConsoleMessage(MessageSource, MessageLevel, String, long, long, String)} for that
     * instead.
     *
     * @param level   The level of the message
     * @param message The message to log
     */
    @Override
    public void logMessage(UltralightLogLevel level, String message) {
        switch (level) {
            case ERROR:
                System.err.println("[Ultralight/ERR] " + message);
                break;

            case WARNING:
                System.err.println("[Ultralight/WARN] " + message);
                break;

            case INFO:
                System.out.println("[Ultralight/INFO] " + message);
                break;
        }
    }
}
