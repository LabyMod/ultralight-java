package net.janrupf.ultralight.javascript.interop;

import net.janrupf.ultralight.annotation.NativeCall;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.javascript.JavascriptContext;
import net.janrupf.ultralight.javascript.JavascriptObject;
import net.janrupf.ultralight.javascript.JavascriptValue;

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
     *         value, construct a {@link JavascriptValue} with the {@link net.janrupf.ultralight.javascript.JavascriptType#NULL}
     *         type
     * @throws JavascriptInteropException If an error occurs while retrieving the property
     */
    @NativeCall
    JavascriptValue getJavascriptProperty(JavascriptContext context, JavascriptObject object, String propertyName)
            throws JavascriptInteropException;
}
