package net.labymedia.ultralight.utils;

import net.labymedia.ultralight.Databind;
import net.labymedia.ultralight.context.ContextProvider;
import net.labymedia.ultralight.javascript.JavascriptObject;

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
