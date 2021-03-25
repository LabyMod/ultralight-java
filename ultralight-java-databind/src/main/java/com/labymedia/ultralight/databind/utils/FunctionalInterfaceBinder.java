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

package com.labymedia.ultralight.databind.utils;

import com.labymedia.ultralight.databind.Databind;
import com.labymedia.ultralight.databind.context.ContextProvider;
import com.labymedia.ultralight.javascript.JavascriptObject;

import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

public class FunctionalInterfaceBinder {

    /**
     * Binds a Javascript object to a functional interface.
     *
     * @param databind The databind instance to use
     * @param target   The type of the interface to bind
     * @param function The function to bind to the interface
     * @return The bound interface
     * @throws IllegalArgumentException If binding the interface fails
     */
    public static Object bind(Databind databind, Class<?> target, JavascriptObject function) {
        if (!target.isAnnotationPresent(FunctionalInterface.class)) {
            throw new IllegalArgumentException(target.getName() + " is not a functional interface");
        } else if (!function.isFunction()) {
            throw new IllegalArgumentException(function.toString() + " is not a function");
        }

        Method bindMethod = null;

        // Search for a matching method
        for (Method m : target.getMethods()) {
            if (m.isDefault()) {
                // Default methods are not suitable
                continue;
            }

            bindMethod = m;
        }

        if (bindMethod == null) {
            throw new IllegalArgumentException(target.getName() + " only contains default methods");
        }

        // Bind a context provider
        ContextProvider contextProvider = databind.getContextProviderFactory().bindProvider(function);
        return Proxy.newProxyInstance(target.getClassLoader(), new Class[]{target},
                new FunctionalInvocationHandler(function, target, contextProvider, databind));
    }

}
