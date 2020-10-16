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

package net.labymedia.ultralight.call;

import net.labymedia.ultralight.api.InjectJavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptObject;
import net.labymedia.ultralight.javascript.JavascriptValue;
import net.labymedia.ultralight.utils.JavascriptConversionUtils;

import java.lang.reflect.Executable;
import java.lang.reflect.Parameter;
import java.util.*;

/**
 * Default implementation of a {@link MethodChooser} selecting methods based on type inference and heuristics.
 */
public final class HeuristicMethodChooser implements MethodChooser {
    @Override
    public <T extends Executable> CallData<T> choose(
            Collection<? extends T> possibilities, JavascriptValue... javascriptValues) {
        // Make space for all parameter types
        Class<?>[] parameterTypes = new Class<?>[javascriptValues.length];

        // Try to find out the Java types of all Javascript parameters
        for (int i = 0; i < parameterTypes.length; i++) {
            parameterTypes[i] = JavascriptConversionUtils.determineType(javascriptValues[i]);
        }

        // Hand of the actual selection to the extended call
        return choose(possibilities, parameterTypes, javascriptValues);
    }

    @Override
    public <T extends Executable> CallData<T> choose(
            Collection<? extends T> possibilities, Class<?>[] sourceParameterTypes, JavascriptValue[] javascriptValues) {
        // The default penalty needs to be higher than the maximum penalty produced by the selection process
        int penalty = Integer.MAX_VALUE;
        Set<CallData<T>> availableMethods = new HashSet<>();

        tryNextMethod:
        for (T executable : possibilities) {
            // It is required to know if we need to shift parameter selection by one if the method
            // needs a Javascript context
            boolean injectContext = executable.isAnnotationPresent(InjectJavascriptContext.class);

            int currentPenalty = 0;
            CallData.VarArgsType varArgsType = null;

            Parameter[] parameters = executable.getParameters();
            if (parameters.length != sourceParameterTypes.length + (injectContext ? 1 : 0)) {
                // Parameter count does not match
                if (!executable.isVarArgs() || sourceParameterTypes.length < parameters.length - (injectContext ? 0 : 1)) {
                    // The method is either not a var args executable or even when the var args are not filled, the
                    // amount of arguments is not enough
                    continue;
                }
            }

            for (int i = 0; i < parameters.length - (injectContext ? 1 : 0); i++) {
                if (i + (injectContext ? 1 : 0) == parameters.length - 1 && executable.isVarArgs()) {
                    // Last parameter is var args, special handling required
                    if (sourceParameterTypes.length < parameters.length) {
                        // Var args not supplied at all
                        varArgsType = CallData.VarArgsType.EMPTY;
                    } else if (sourceParameterTypes[i].isArray() && sourceParameterTypes.length == parameters.length) {
                        if (parameters[i + (injectContext ? 1 : 0)].getType()
                                .isAssignableFrom(sourceParameterTypes[i])) {
                            // Var args array can be passed through as the array types match
                            varArgsType = CallData.VarArgsType.PASS_THROUGH;
                        } else {
                            if (!parameters[i + (injectContext ? 1 : 0)].getType().getComponentType()
                                    .isAssignableFrom(sourceParameterTypes[i])) {
                                // Method parameter can't be compacted down
                                continue tryNextMethod;
                            }
                            // Compacting the last argument is enough to make it compatible
                            varArgsType = CallData.VarArgsType.COMPACT;
                        }

                        // Apply var args penalty
                        currentPenalty += 1000;
                    } else {
                        // The arguments need to be compacted
                        varArgsType = CallData.VarArgsType.COMPACT;

                        for (int x = i; x < sourceParameterTypes.length; x++) {
                            // Sum up the penalties for every method parameter
                            int argPenalty = calculatePenalty(
                                    parameters[i + (injectContext ? 1 : 0)].getType().getComponentType(),
                                    sourceParameterTypes[x],
                                    javascriptValues[x]
                            );
                            if (argPenalty < 0) {
                                // Argument is incompatible, search next method
                                continue tryNextMethod;
                            }

                            currentPenalty += argPenalty;
                        }

                        // Apply var args penalty
                        currentPenalty += 1000;
                    }
                } else {
                    int argPenalty = calculatePenalty(
                            parameters[i + (injectContext ? 1 : 0)].getType(),
                            sourceParameterTypes[i],
                            javascriptValues[i]
                    );
                    if (argPenalty < 0) {
                        // Argument is incompatible, search next method
                        continue tryNextMethod;
                    }

                    currentPenalty += argPenalty;
                }
            }

            if (currentPenalty < penalty) {
                // Found a better matching implementation, clear available ones
                availableMethods.clear();
            }

            availableMethods.add(new CallData<>(executable, varArgsType));
        }

        if (availableMethods.size() > 1) {
            throw new IllegalStateException("Ambiguous argument types, could not determine methods");
        } else if (availableMethods.isEmpty()) {
            throw new IllegalStateException("No matching method found");
        }

        return availableMethods.iterator().next();
    }

    private int calculatePenalty(Class<?> target, Class<?> source, JavascriptValue value) {
        if (target == JavascriptValue.class) {
            // No casting required at all, as the value can be passed directly
            return 0;
        } else if (target == JavascriptObject.class) {
            // If the request type is a JavascriptObject, it can only be fulfilled if the value is an object
            return value.isObject() ? 0 : -1;
        } else if (isZeroCostConversion(target, source)) {
            // Special zero cost conversion
            return 0;
        }  else if (target == source) {
            // No casting required, fast case to not run through selection
            return 0;
        } else if (source == JavascriptObject.class &&
                target.isAnnotationPresent(FunctionalInterface.class) &&
                value.toObject().isFunction()) {
            // Functional conversion possible
            return 0;
        } else if (!target.isAssignableFrom(source)) {
            // Conversion is not possible at all
            return -1;
        }

        /*
         * Dijkstra algorithm:
         * Used for finding the shortest conversion path of Java classes from one to another.
         *
         * Initial setup, use the current source as the starting node
         */
        HashMap<Class<?>, Integer> dist = new HashMap<>();
        Queue<ClassWithPriority> queue = new LinkedList<>();
        queue.add(new ClassWithPriority(source, 0));

        while (!queue.isEmpty()) {
            // There are path left, check them
            ClassWithPriority cwp = queue.remove();
            List<ClassWithPriority> next = new ArrayList<>();

            if (cwp.clazz.getSuperclass() != null && target.isAssignableFrom(cwp.clazz.getSuperclass())) {
                // Found a path, increment length of the path and add it
                next.add(new ClassWithPriority(cwp.clazz.getSuperclass(), cwp.priority + 1));
            }

            // Also check if for all interfaces, incrementing the length by one for each path
            Arrays.stream(cwp.clazz.getInterfaces())
                    .filter(target::isAssignableFrom)
                    .map(clazz -> new ClassWithPriority(clazz, cwp.priority + 1))
                    .forEach(next::add);

            next.forEach(e -> {
                // Check if a path is better
                if (e.priority < dist.getOrDefault(e.clazz, Integer.MAX_VALUE)) {
                    // Found a better one, select it and put it into the queue
                    dist.put(e.clazz, e.priority);
                    queue.add(e);
                }
            });
        }

        return dist.get(target);
    }

    /**
     * Catches special cases such as conversions from {@link Number} to {@code int}.
     *
     * @param target The class to convert to
     * @param source The class to convert from
     * @return {@code true} if a zero cost conversion is possible, {@code false} otherwise
     */
    private boolean isZeroCostConversion(Class<?> target, Class<?> source) {
        // Number to primitive
        if (source == Number.class && (target == byte.class ||
                target == short.class ||
                target == int.class ||
                target == long.class ||
                target == float.class ||
                target == double.class
        )) {
            return true;
        } else if (source == Boolean.class && target == boolean.class) {
            return true;
        } else {
            return source == Character.class && target == char.class;
        }
    }

    /**
     * Helper tuple to combine a class with a priority for the dijkstra algorithm.
     */
    private static class ClassWithPriority {
        Class<?> clazz;
        int priority;

        public ClassWithPriority(Class<?> clazz, int priority) {
            this.clazz = clazz;
            this.priority = priority;
        }
    }
}
