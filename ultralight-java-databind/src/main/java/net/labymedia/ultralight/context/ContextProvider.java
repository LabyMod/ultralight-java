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

package net.labymedia.ultralight.context;

import net.labymedia.ultralight.javascript.JavascriptContextLock;

/**
 * Interface to be implemented by the user.
 */
public interface ContextProvider {
    /**
     * Retrieves the bound Javascript context with a lock. The lock will be unlocked by the consumer automatically
     * after it finished using it.
     *
     * @return The bound Javascript context lock, or {@code null}, if the context is not available anymore
     */
    JavascriptContextLock getContext();
}
