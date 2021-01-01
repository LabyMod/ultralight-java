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

package com.labymedia.ultralight.api;

import com.labymedia.ultralight.Databind;
import com.labymedia.ultralight.DatabindJavascriptClass;
import com.labymedia.ultralight.javascript.JavascriptContext;
import com.labymedia.ultralight.javascript.JavascriptObject;

/**
 * Base API for java operations from within javascript.
 */
public final class JavaAPI {
    private final Databind databind;

    /**
     * Constructs a new base API using the given {@link Databind} instance.
     *
     * @param databind The databind instance to use for translating classes
     */
    public JavaAPI(Databind databind) {
        this.databind = databind;
    }

    /**
     * Imports a Java class and makes it available to Javascript.
     *
     * @param context The javascript context to use for converting
     * @param className The fully qualified java class name to import
     * @return The imported java class
     * @throws ClassNotFoundException If the java class can not be found
     */
    @InjectJavascriptContext
    public JavascriptObject importClass(JavascriptContext context, String className) throws ClassNotFoundException {
        Class<?> javaClass = Class.forName(className);

        return context.makeObject(databind.toJavascript(javaClass),
                new DatabindJavascriptClass.Data(null, javaClass));
    }
}
