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

import com.labymedia.ultralight.databind.api.InjectJavascriptContext;
import com.labymedia.ultralight.databind.utils.JavascriptConversionUtils;
import com.labymedia.ultralight.javascript.JavascriptContext;
import com.labymedia.ultralight.javascript.JavascriptValue;

import java.lang.reflect.Array;
import java.lang.reflect.Executable;
import java.lang.reflect.Parameter;
import java.util.ArrayList;
import java.util.List;

/**
 * Class containing information on how to call certain {@link Executable}s from Javascript for a given set of arguments.
 *
 * @param <T> The type this call information may be applied to
 */
public class CallData<T extends Executable> {
    private final T target;
    private final VarArgsType varArgsType;

    /**
     * Constructs a new {@link CallData} for a certain target specifying how to handle var args.
     *
     * @param target      The target of this call
     * @param varArgsType The type of the var args expansion
     */
    public CallData(T target, VarArgsType varArgsType) {
        this.target = target;
        this.varArgsType = varArgsType;
    }

    /**
     * Retrieves the target of this call data.
     *
     * @return The target executable
     */
    public T getTarget() {
        return target;
    }

    /**
     * Constructs the arguments based on the requested call data.
     *
     * @param context         The Javascript context to use for argument construction
     * @param conversionUtils The conversion utilities used for converting objects
     * @param arguments       The Javascript arguments
     * @return The List of constructed arguments
     */
    public List<Object> constructArguments(
            JavascriptContext context, JavascriptConversionUtils conversionUtils, JavascriptValue[] arguments) {
        Parameter[] methodParameters = target.getParameters();
        List<Object> parameters = new ArrayList<>();

        boolean injectContext = target.isAnnotationPresent(InjectJavascriptContext.class);

        // Convert all parameters to Javascript values
        for (int i = 0; i < methodParameters.length; i++) {
            if (i == 0 && injectContext) {
                parameters.add(context);
            } else if (i == methodParameters.length - 1 && target.isVarArgs()) {
                switch (varArgsType) {
                    case NONE:
                        // No special handling required
                        break;

                    case EMPTY:
                        // Supply empty array
                        parameters.add(Array.newInstance(methodParameters[i].getType().getComponentType(), 0));
                        break;

                    case COMPACT:
                        // Compact remaining variable arguments into an array
                        int varArgsCount = arguments.length - i;
                        Class<?> targetType = methodParameters[i].getType().getComponentType();

                        Object args = Array.newInstance(targetType, varArgsCount);

                        for (int x = 0; x < varArgsCount; x++) {
                            Array.set(args, x, conversionUtils.fromJavascript(
                                    arguments[(i + x) - (injectContext ? 1 : 0)], targetType));
                        }

                        parameters.add(args);
                        break;

                    case PASS_THROUGH:
                        // Pass through the argument one to one
                        parameters.add(conversionUtils.fromJavascript(
                                arguments[i - (injectContext ? 1 : 0)], methodParameters[i].getType()));
                        break;
                }
            } else {
                // Single argument
                parameters.add(conversionUtils.fromJavascript(
                        arguments[i - (injectContext ? 1 : 0)], methodParameters[i].getType()));
            }
        }

        return parameters;
    }

    /**
     * Describes how to expand var args for a certain call.
     */
    public enum VarArgsType {
        /**
         * Target does not support var args at all.
         */
        NONE,

        /**
         * Compact parameters into an array for the call.
         */
        COMPACT,

        /**
         * Pass parameters one to one to the method.
         */
        PASS_THROUGH,

        /**
         * Leave the var args empty.
         */
        EMPTY
    }
}
