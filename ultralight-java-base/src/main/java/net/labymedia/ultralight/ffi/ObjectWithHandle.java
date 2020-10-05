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

package net.labymedia.ultralight.ffi;

import net.labymedia.ultralight.annotation.NativeCall;
import net.labymedia.ultralight.annotation.NativeType;
import net.labymedia.ultralight.annotation.Unsigned;

/**
 * Interface used by native code to retrieve underlying pointers
 * from java instances,
 */
public interface ObjectWithHandle {
    /**
     * Retrieves the underlying native pointer from this instance.
     *
     * @return The underlying native pointer
     */
    @NativeCall
    @NativeType("void *")
    @Unsigned
    long getHandle();
}
