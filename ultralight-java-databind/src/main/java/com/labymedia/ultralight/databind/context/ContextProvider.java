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

package com.labymedia.ultralight.databind.context;

import com.labymedia.ultralight.javascript.JavascriptContextLock;

import java.util.function.Consumer;

/**
 * Interface to be implemented by the user.
 */
public interface ContextProvider {
    /**
     * Executes the callback later on a thread which is synchronized with the Javascript engine.
     *
     * @param callback The callback to execute
     */
    void syncWithJavascript(Consumer<JavascriptContextLock> callback);
}
