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

package net.labymedia.ultralight;

import net.labymedia.ultralight.call.CallData;
import net.labymedia.ultralight.call.MethodChooser;
import net.labymedia.ultralight.javascript.*;
import net.labymedia.ultralight.javascript.interop.JavascriptInteropException;
import net.labymedia.ultralight.utils.JavascriptConversionUtils;

import java.lang.reflect.*;
import java.util.*;

/**
 * Representation of a translated Java class.
 */
public final class DatabindJavascriptClass {
    private final JavascriptClassDefinition definition;

    private final DatabindConfiguration configuration;
    private final JavascriptConversionUtils conversionUtils;
    private final MethodChooser methodChooser;

    private final Set<Constructor<?>> constructors = new HashSet<>();
    private final Map<String, Set<Method>> methods = new HashMap<>();
    private final Map<String, Field> fields = new HashMap<>();

    /**
     * Constructs a new {@link DatabindJavascriptClass}.
     *
     * @param configuration   The configuration to use
     * @param conversionUtils The conversion utilities to use for converting objects
     * @param className       The name of the Javascript class
     */
    private DatabindJavascriptClass(
            DatabindConfiguration configuration, JavascriptConversionUtils conversionUtils, String className) {
        this.definition = new JavascriptClassDefinition()
                .name(className)
                .attributes(JavascriptClassAttributes.NO_AUTOMATIC_PROTOTYPE);

        this.configuration = configuration;
        this.conversionUtils = conversionUtils;
        this.methodChooser = configuration.methodChooser();
    }

    /**
     * Registers the callbacks on the definition.
     */
    private void registerCallbacks() {
        definition.onCallAsConstructor(this::onCallAsConstructor);
        definition.onHasProperty(this::onHasProperty);
        definition.onGetProperty(this::onGetProperty);
        definition.onSetProperty(this::onSetProperty);
    }

    /**
     * Indexes constructors into this class.
     *
     * @param constructors The constructors to index
     */
    private void addConstructors(Constructor<?>... constructors) {
        this.constructors.addAll(Arrays.asList(constructors));
    }

    /**
     * Indexes methods into this class.
     *
     * @param methods The methods to index
     */
    private void addMethods(Method... methods) {
        for (Method method : methods) {
            String name = method.getName();

            if (this.methods.containsKey(name)) {
                // Overloaded method, add it to the set of methods with the same name
                this.methods.get(name).add(method);
                continue;
            }

            // Create a set to store possible overloads in and add the found method
            Set<Method> methodSet = new HashSet<>();
            methodSet.add(method);

            this.methods.put(name, methodSet);
        }
    }

    /**
     * Indexes fields into this class.
     *
     * @param fields The fields to index
     */
    private void addFields(Field... fields) {
        for (Field field : fields) {
            this.fields.put(field.getName(), field);
        }
    }

    /**
     * Called by Javascript when this class is instantiated using {@code new}.
     *
     * @param context     The context the instantiation is called in
     * @param constructor The constructor that is being called
     * @param arguments   The arguments that are being passed to the constructor
     * @return The constructed instance as a Javascript value
     * @throws JavascriptInteropException If the construction fails
     */
    private JavascriptObject onCallAsConstructor(
            JavascriptContext context,
            JavascriptObject constructor,
            JavascriptValue[] arguments
    ) throws JavascriptInteropException {
        Data privateData = (Data) constructor.getPrivate();
        if (privateData != null && privateData.instance != null) {
            throw new IllegalStateException("Can't call constructor on an already constructed object");
        }

        CallData<Constructor<?>> callData = methodChooser.choose(constructors, arguments);

        // Prepare call
        Constructor<?> method = callData.getTarget();
        List<Object> parameters = callData.constructArguments(
                context,
                conversionUtils,
                arguments
        );

        try {
            // Invoke constructor with constructed arguments
            return context.makeObject(bake(), new Data(method.newInstance(parameters.toArray()), null));
        } catch (IllegalAccessException exception) {
            throw new JavascriptInteropException("Unable to access constructor: " + method.getName(), exception);
        } catch (InvocationTargetException exception) {
            throw new JavascriptInteropException("Constructor threw an exception", exception);
        } catch (InstantiationException exception) {
            throw new JavascriptInteropException("Unable to create instance", exception);
        }
    }

    /**
     * Determines whether a property exists.
     *
     * @param context The context the check is executed in
     * @param object The object to check for the property on
     * @param propertyName The name of the property to check for
     * @return {@code true} if the property could be found, {@code false} otherwise
     */
    private boolean onHasProperty(JavascriptContext context, JavascriptObject object, String propertyName) {
        // Determine whether an instance is available or if the object is static
        boolean instanceAvailable = ((Data) object.getPrivate()).instance != null;

        Field f = fields.get(propertyName);
        if(f != null && (Modifier.isStatic(f.getModifiers()) || instanceAvailable)) {
            // Field found and usable
            return true;
        }

        Set<Method> methodsWithName = methods.get(propertyName);
        if(methodsWithName == null || methodsWithName.isEmpty()) {
            // No methods available with that name
            return false;
        } else if(instanceAvailable) {
            // There is an instance and methods with this name are available
            return true;
        }

        // There are methods with this name available, check if any of them is static
        for(Method method : methodsWithName) {
            if(Modifier.isStatic(method.getModifiers())) {
                // Found a static variant
                return true;
            }
        }

        // No static method available
        return false;
    }

    /**
     * Called by Javascript when a property is requested on this class or on an instance of this class.
     *
     * @param context      The context the property is being requested in
     * @param object       The object the property is being requested on
     * @param propertyName The name of the requested property
     * @return The value of the property, or {@code null}, if the property does not exist
     * @throws JavascriptInteropException If retrieving the property fails
     */
    private JavascriptValue onGetProperty(
            JavascriptContext context, JavascriptObject object, String propertyName) throws JavascriptInteropException {
        Data privateData = (Data) object.getPrivate();
        Field field = fields.get(propertyName);

        if (field != null) {
            try {
                return conversionUtils.toJavascript(context, field.get(privateData.instance), field.getType());
            } catch (IllegalAccessException exception) {
                throw new JavascriptInteropException("Unable to access field: " + field.getName(), exception);
            }
        }

        Set<Method> methodSet = methods.get(propertyName);
        if (methodSet == null) {
            // Property does not exist
            return context.makeUndefined();
        }

        return context.makeObject(
                DatabindJavascriptMethodHandler.create(
                        configuration,
                        conversionUtils,
                        methodSet,
                        propertyName).bake(),
                new DatabindJavascriptMethodHandler.Data(privateData.instance, null));
    }

    /**
     * Called by Javascript when a property is being set.
     *
     * @param context      The context the property is being set in
     * @param object       The object the property is being set on
     * @param propertyName The name of the property being set
     * @param value        The new value of the property
     * @return {@code true} if setting the property is found, {@code false} otherwise
     * @throws JavascriptInteropException If an error is encountered while setting the property
     */
    private boolean onSetProperty(
            JavascriptContext context,
            JavascriptObject object,
            String propertyName,
            JavascriptValue value
    ) throws JavascriptInteropException {
        Data privateData = (Data) object.getPrivate();
        Field field = fields.get(propertyName);

        if (field != null) {
            try {
                field.set(privateData.instance, conversionUtils.fromJavascript(value, field.getType()));
            } catch (IllegalAccessException exception) {
                throw new JavascriptInteropException("Unable to access field: " + field.getName(), exception);
            }
        }

        if (methods.containsKey(propertyName)) {
            throw new UnsupportedOperationException("Can not set a method");
        }

        return false;
    }

    /**
     * Bakes this class definition.
     *
     * @return The baked definition
     */
    public JavascriptClass bake() {
        return definition.bake();
    }

    /**
     * Creates a new bound Javascript class.
     *
     * @param configuration   The configuration to use
     * @param conversionUtils The conversion utilities to use for converting objects
     * @param javaClass       The java class to create a binding for
     * @return The created binding
     */
    static DatabindJavascriptClass create(
            DatabindConfiguration configuration, JavascriptConversionUtils conversionUtils, Class<?> javaClass) {
        DatabindJavascriptClass javascriptClass = new DatabindJavascriptClass(
                configuration, conversionUtils, javaClass.getName());

        javascriptClass.registerCallbacks();
        javascriptClass.addConstructors(javaClass.getDeclaredConstructors());
        javascriptClass.addMethods(javaClass.getDeclaredMethods());
        javascriptClass.addFields(javaClass.getDeclaredFields());

        // Iteratively scan all interfaces
        Queue<Class<?>> toAdd = new LinkedList<>(Arrays.asList(javaClass.getInterfaces()));
        while (!toAdd.isEmpty()) {
            Class<?> iface = toAdd.remove();
            toAdd.addAll(Arrays.asList(iface.getInterfaces()));

            javascriptClass.addMethods(iface.getMethods());
        }

        return javascriptClass;
    }

    /**
     * Represents the internal state of a Javascript class.
     */
    public static class Data {
        private final Object instance;
        private final Class<?> javaClass;

        /**
         * Constructs a new {@link Data} instance.
         *
         * @param instance  The Java instance of this class, or {@code null}, if none
         * @param javaClass The Java class of this class, or {@code null}, if none
         */
        public Data(Object instance, Class<?> javaClass) {
            this.instance = instance;
            this.javaClass = javaClass;
        }

        /**
         * Retrieves the Java instance this class represents.
         *
         * @return The java instance this class represents, or {@code null}, if none
         */
        public Object instance() {
            return instance;
        }

        /**
         * Retrieves the Java class this class represents.
         *
         * @return The Java class this class represents, or {@code null}, if none
         */
        public Class<?> javaClass() {
            return javaClass;
        }
    }
}
