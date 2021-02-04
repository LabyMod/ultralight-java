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

package com.labymedia.ultralight.databind.call;

import com.labymedia.ultralight.javascript.JavascriptValue;

import java.lang.reflect.Executable;
import java.util.Collection;

/**
 * Describes an abstraction layer around the system determining which method to invoke on an incoming
 * call from Javascript.
 */
public interface MethodChooser {
    /**
     * Chooses the best matching method based on the arguments incoming from Javascript.
     *
     * @param possibilities    All executables which could possibly be targeted by this call
     * @param javascriptValues The arguments being passed from Javascript
     * @param <T>              The target executable type
     * @return Information on how to call the chosen executable
     * @throws IllegalStateException If the target executable can not be determined
     */
    <T extends Executable> CallData<T> choose(Collection<? extends T> possibilities, JavascriptValue[] javascriptValues);

    /**
     * Chooses the best matching method based on the arguments incoming from Javascript.
     *
     * @param possibilities        All executables which could possibly be targeted by this call
     * @param sourceParameterTypes The desired types for the parameters
     * @param javascriptValues     The arguments being passed from Javascript
     * @param <T>                  The target executable type
     * @return Information on how to call the chosen executable
     * @throws IllegalStateException If the target executable can not be determined
     */
    <T extends Executable> CallData<T> choose(
            Collection<? extends T> possibilities, Class<?>[] sourceParameterTypes, JavascriptValue[] javascriptValues);
}
