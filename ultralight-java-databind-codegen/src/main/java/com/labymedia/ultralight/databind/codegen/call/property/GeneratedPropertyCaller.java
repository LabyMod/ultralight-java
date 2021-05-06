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

import com.labymedia.ultralight.databind.call.property.PropertyCaller;
import com.labymedia.ultralight.databind.call.property.ReflectivePropertyCaller;
import com.labymedia.ultralight.javascript.interop.JavascriptInteropException;
import javassist.CannotCompileException;
import javassist.NotFoundException;

import java.io.IOException;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

/**
 * Calls properties on java objects or classes via lazy-bytecode-generated {@link SingleGeneratedPropertyCaller}s.
 * This implementation is recommended if there are a lot of calls in a short time on the same property.
 * Although this implementation is faster than {@link ReflectivePropertyCaller},
 * the first call might be slower because of the lazy generation.
 */
public class GeneratedPropertyCaller implements PropertyCaller {

    /**
     * All already generated {@link SingleGeneratedPropertyCaller}s for methods.
     */
    private final Map<Method, SingleGeneratedPropertyCaller> methodCallers;
    /**
     * All already generated {@link SingleGeneratedPropertyCaller}s for constructors.
     */
    private final Map<Constructor<?>, SingleGeneratedPropertyCaller> constructorCallers;
    /**
     * All already generated {@link SingleGeneratedPropertyCaller}s for fields.
     */
    private final Map<Field, SingleGeneratedPropertyCaller> fieldCallers;

    /**
     * Generator for new {@link SingleGeneratedPropertyCaller}s.
     */
    private final PropertyCallerGenerator callerGenerator;

    private GeneratedPropertyCaller() {
        this.methodCallers = new HashMap<>();
        this.constructorCallers = new HashMap<>();
        this.fieldCallers = new HashMap<>();

        this.callerGenerator = new PropertyCallerGenerator();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Object callMethod(Object instance, Method method, Object[] parameters) throws JavascriptInteropException {
        SingleGeneratedPropertyCaller propertyCaller = this.methodCallers.computeIfAbsent(method, key -> {
            try {
                return this.callerGenerator.generateMethodCaller(method);
            } catch (NotFoundException | CannotCompileException | IllegalAccessException | InstantiationException | NoSuchMethodException | InvocationTargetException | IOException exception) {
                throw new CallerGenerationException("Failed to generate caller for method " + method.getName(), exception);
            }
        });

        try {
            return propertyCaller.callProperty(instance, parameters);
        } catch (Exception exception) {
            throw new JavascriptInteropException(method.getName() + " threw an exception", exception);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Object callConstructor(Constructor<?> constructor, Object[] parameters) throws JavascriptInteropException {
        SingleGeneratedPropertyCaller propertyCaller = this.constructorCallers.computeIfAbsent(constructor, key -> {
            try {
                return this.callerGenerator.generateConstructorCaller(key);
            } catch (NotFoundException | IllegalAccessException | InstantiationException | NoSuchMethodException | InvocationTargetException | IOException | CannotCompileException exception) {
                throw new CallerGenerationException("Failed to generate caller for constructor", exception);
            }
        });

        try {
            return propertyCaller.callProperty(null, parameters);
        } catch (Exception exception) {
            throw new JavascriptInteropException("Constructor call threw an exception", exception);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public Object callFieldGet(Object instance, Field field) throws JavascriptInteropException {
        SingleGeneratedPropertyCaller propertyCaller = this.fieldCallers.computeIfAbsent(field, key -> {
            try {
                return this.callerGenerator.generateFieldCaller(key);
            } catch (NotFoundException | CannotCompileException | IllegalAccessException | InstantiationException | NoSuchMethodException | InvocationTargetException | IOException exception) {
                throw new CallerGenerationException("Failed to generate caller for field " + field.getName(), exception);
            }
        });

        try {
            return propertyCaller.callProperty(instance, null);
        } catch (Exception exception) {
            throw new JavascriptInteropException("Field call for " + field.getName() + " threw an exception", exception);
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void callFieldSet(Object instance, Field field, Object value) throws JavascriptInteropException {
        SingleGeneratedPropertyCaller propertyCaller = this.fieldCallers.computeIfAbsent(field, key -> {
            try {
                return this.callerGenerator.generateFieldCaller(key);
            } catch (NotFoundException | CannotCompileException | IllegalAccessException | InstantiationException | NoSuchMethodException | InvocationTargetException | IOException exception) {
                throw new CallerGenerationException("Failed to generate caller for field " + field.getName(), exception);
            }
        });

        try {
            propertyCaller.callProperty(instance, new Object[]{value});
        } catch (Exception exception) {
            throw new JavascriptInteropException("Field call for " + field.getName() + " threw an exception", exception);
        }
    }

    /**
     * Factory for {@link GeneratedPropertyCaller}.
     */
    public static class Factory implements PropertyCaller.Factory {

        /**
         * @return A new {@link GeneratedPropertyCaller} instance
         */
        @Override
        public GeneratedPropertyCaller create() {
            return new GeneratedPropertyCaller();
        }
    }
}
