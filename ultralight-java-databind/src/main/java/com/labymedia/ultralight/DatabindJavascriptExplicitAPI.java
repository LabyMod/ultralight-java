/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2021 LabyMedia and contributors
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

package com.labymedia.ultralight;

import com.labymedia.ultralight.javascript.*;
import com.labymedia.ultralight.javascript.*;
import com.labymedia.ultralight.utils.JavascriptConversionUtils;

import java.util.ArrayList;
import java.util.List;

/**
 * Helper for calling methods using explicit signature definition.
 */
public final class DatabindJavascriptExplicitAPI {
    private final JavascriptClassDefinition definition;
    private final JavascriptConversionUtils conversionUtils;

    /**
     * Constructs a new {@link DatabindJavascriptExplicitAPI} instance.
     *
     * @param conversionUtils The instance of {@link JavascriptConversionUtils} used for converting values
     * @param name            The name of the class
     */
    private DatabindJavascriptExplicitAPI(JavascriptConversionUtils conversionUtils, String name) {
        this.definition = new JavascriptClassDefinition()
                .name(name)
                .attributes(JavascriptClassAttributes.NO_AUTOMATIC_PROTOTYPE);

        this.conversionUtils = conversionUtils;
    }

    /**
     * Registers the callbacks on the definition
     */
    private void registerCallbacks() {
        definition.onCallAsFunction(this::onCallAsFunction);
    }

    /**
     * Called by Javascript when the explicit API is invoked to bind a function.
     *
     * @param context    The context the function is invoked in
     * @param function   The function being invoked
     * @param thisObject The {@code this} argument of the invocation
     * @param arguments  The arguments passed to the function
     * @return The parameter type bound function
     */
    private JavascriptValue onCallAsFunction(
            JavascriptContext context,
            JavascriptObject function,
            JavascriptObject thisObject,
            JavascriptValue[] arguments
    ) {
        Data privateData = (Data) function.getPrivate();
        List<Class<?>> parameterTypes = new ArrayList<>();

        // Convert all Javascript arguments to Java arguments
        for (JavascriptValue value : arguments) {
            Object object = conversionUtils.fromJavascript(value, JavascriptConversionUtils.determineType(value));
            parameterTypes.add((Class<?>) object);
        }

        // Create the bound function
        return context.makeObject(privateData.methodHandler(),
                new DatabindJavascriptMethodHandler.Data(privateData.instance, parameterTypes.toArray(new Class[0])));
    }

    /**
     * Bakes the class definition.
     *
     * @return The baked class
     */
    public JavascriptClass bake() {
        return definition.bake();
    }

    /**
     * Creates the explicit API using the given conversion utils and class name.
     *
     * @param conversionUtils The conversion utils to use to convert objects
     * @param name            The name of the Javascript class
     * @return The created explicit API
     */
    static DatabindJavascriptExplicitAPI create(JavascriptConversionUtils conversionUtils, String name) {
        DatabindJavascriptExplicitAPI javascriptClass = new DatabindJavascriptExplicitAPI(conversionUtils, name);
        javascriptClass.registerCallbacks();
        return javascriptClass;
    }

    /**
     * Data of a parameter type bound call.
     */
    public static class Data {
        private final Object instance;
        private final JavascriptClass methodHandler;

        /**
         * Constructs a new {@link Data} instance for a bound call.
         *
         * @param instance      The instance the call is being issued on
         * @param methodHandler The method handler used for issuing the call
         */
        public Data(Object instance, JavascriptClass methodHandler) {
            this.instance = instance;
            this.methodHandler = methodHandler;
        }

        /**
         * The instance this call is being issued on.
         *
         * @return The instance this call is being issue on
         */
        private Object instance() {
            return instance;
        }

        /**
         * The method handler used for issuing the call.
         *
         * @return The method handler used for issuing the call
         */
        public JavascriptClass methodHandler() {
            return methodHandler;
        }
    }
}
