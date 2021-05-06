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

package com.labymedia.ultralight.databind.codegen.call.property;

/**
 * Represents a bytecode-generated caller which directly calls a certain property on a java object or class without reflection.
 */
public interface SingleGeneratedPropertyCaller {

    /**
     * Calls the property of the object or class directly.
     *
     * @param instance   The instance the property belongs to or {@code null} if the property belongs to a class.
     * @param parameters The parameters the property should be called with or {@code null} if the property does not support arguments.
     * @return The result of the call or {@code null} if the property has no result
     */
    Object callProperty(Object instance, Object[] parameters);
}
