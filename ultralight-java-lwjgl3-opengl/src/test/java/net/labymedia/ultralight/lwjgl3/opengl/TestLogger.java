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

import net.labymedia.ultralight.plugin.logging.UltralightLogLevel;
import net.labymedia.ultralight.plugin.logging.UltralightLogger;

/**
 * Test implementation of a logger
 */
public class TestLogger implements UltralightLogger {
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
