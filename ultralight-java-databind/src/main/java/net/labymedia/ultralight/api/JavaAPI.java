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

package net.labymedia.ultralight.api;

import net.labymedia.ultralight.Databind;
import net.labymedia.ultralight.DatabindJavascriptClass;
import net.labymedia.ultralight.javascript.JavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptObject;

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