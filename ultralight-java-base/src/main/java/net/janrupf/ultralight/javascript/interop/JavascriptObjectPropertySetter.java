package net.janrupf.ultralight.javascript.interop;

import net.janrupf.ultralight.annotation.NativeCall;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.javascript.JavascriptContext;
import net.janrupf.ultralight.javascript.JavascriptObject;
import net.janrupf.ultralight.javascript.JavascriptValue;

/**
 * Callback for setting properties on Javascript objects.
 */
@NativeType("JSObjectSetPropertyCallback")
public interface JavascriptObjectPropertySetter {
    /**
     * The callback invoked when setting a property's value.
     * <p>
     * If this function returns false, the set request forwards to object's statically declared properties, then its
     * parent class chain (which includes the default object class).
     *
     * @param context      The execution context to use
     * @param object       The object on which to set the property's value
     * @param propertyName A string containing the name of the property to set
     * @param value        A value to use as the property's value
     * @return {@code true} if the property was set, otherwise {@code false}
     * @throws JavascriptInteropException If an error occurs while setting the property
     */
    @NativeCall
    boolean setJavascriptProperty(
            JavascriptContext context, JavascriptObject object, String propertyName, JavascriptValue value)
            throws JavascriptInteropException;
}
