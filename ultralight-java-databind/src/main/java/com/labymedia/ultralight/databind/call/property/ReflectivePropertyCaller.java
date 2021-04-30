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

package com.labymedia.ultralight.databind.call.property;

import com.labymedia.ultralight.javascript.interop.JavascriptInteropException;

import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class ReflectivePropertyCaller implements PropertyCaller {

    @Override
    public Object callMethod(Object instance, Method method, Object[] parameters) throws JavascriptInteropException {
        try {
            return method.invoke(instance, parameters);
        } catch (IllegalAccessException exception) {
            throw new JavascriptInteropException("Unable to access method: " + method.getName(), exception);
        } catch (InvocationTargetException exception) {
            throw new JavascriptInteropException(method.getName() + " threw an exception", exception);
        }
    }

    @Override
    public Object callConstructor(Constructor<?> constructor, Object[] parameters) throws JavascriptInteropException {
        try {
            return constructor.newInstance(parameters);
        } catch (IllegalAccessException exception) {
            throw new JavascriptInteropException("Unable to access constructor: " + constructor.getName(), exception);
        } catch (InvocationTargetException exception) {
            throw new JavascriptInteropException("Constructor threw an exception", exception);
        } catch (InstantiationException exception) {
            throw new JavascriptInteropException("Unable to create instance", exception);
        }
    }

    @Override
    public Object callFieldGet(Object instance, Field field) throws JavascriptInteropException {
        try {
            return field.get(instance);
        } catch (IllegalAccessException exception) {
            throw new JavascriptInteropException("Unable to access field: " + field.getName(), exception);
        }
    }

    @Override
    public void callFieldSet(Object instance, Field field, Object value) throws JavascriptInteropException {
        try {
            field.set(instance, value);
        } catch (IllegalAccessException exception) {
            throw new JavascriptInteropException("Unable to access field: " + field.getName(), exception);
        }
    }

    public static class Factory implements PropertyCaller.Factory {

        @Override
        public ReflectivePropertyCaller create() {
            return new ReflectivePropertyCaller();
        }
    }
}
