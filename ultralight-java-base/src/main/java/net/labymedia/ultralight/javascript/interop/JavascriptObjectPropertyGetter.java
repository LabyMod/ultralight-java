package net.labymedia.ultralight.javascript.interop;

import net.labymedia.ultralight.annotation.NativeCall;
import net.labymedia.ultralight.annotation.NativeType;
import net.labymedia.ultralight.javascript.JavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptObject;
import net.labymedia.ultralight.javascript.JavascriptValue;
import net.labymedia.ultralight.javascript.JavascriptType;

/**
 * Callback for retrieving properties from a Javascript object.
 */
@NativeType("JSObjectGetPropertyCallback")
public interface JavascriptObjectPropertyGetter {
    /**
     * The callback invoked when getting a property's value.
     * <p>
     * If this function returns {@code null}, the get request forwards to object's statically declared properties, then
     * its parent class chain (which includes the default object class), then its prototype chain.
     *
     * @param context      The execution context to use
     * @param object       The object to search for the property
     * @param propertyName A string containing the name of the property to get
     * @return The property's value if object has the property, otherwise {@code null}, to return the literal null
     *         value, construct a {@link JavascriptValue} with the {@link JavascriptType#NULL}
     *         type
     * @throws JavascriptInteropException If an error occurs while retrieving the property
     */
    @NativeCall
    JavascriptValue getJavascriptProperty(JavascriptContext context, JavascriptObject object, String propertyName)
            throws JavascriptInteropException;
}
