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

package net.labymedia.ultralight.annotation;

import java.lang.annotation.Documented;

/**
 * Marks an element as subject to change. This might be used when it is not yet clear if an implementation will be kept
 * or changed.
 */
@Documented
public @interface Unstable {
    /**
     * Specifies the reason this element has been marked as unstable.
     *
     * @return The reason this element is unstable
     */
    String value();
}
