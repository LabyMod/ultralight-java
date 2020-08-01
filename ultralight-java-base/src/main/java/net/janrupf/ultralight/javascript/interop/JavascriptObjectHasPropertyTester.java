package net.janrupf.ultralight.javascript.interop;

import net.janrupf.ultralight.annotation.NativeCall;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.javascript.JavascriptContext;
import net.janrupf.ultralight.javascript.JavascriptObject;

/**
 * Callback for testing if a Javascript object has a certain property.
 */
@NativeType("JSObjectHasPropertyCallback")
public interface JavascriptObjectHasPropertyTester {
    /**
     * If this function returns false, the hasProperty request forwards to object's statically declared properties, then
     * its parent class chain (which includes the default object class), then its prototype chain.
     * <p>
     * This callback enables optimization in cases where only a property's existence needs to be known, not its value,
     * and computing its value would be expensive.
     *
     * @param context The execution context to use
     * @param object The object to search for the property
     * @param propertyName A string containing the name of the property look up
     * @return {@code true} if object has the property, otherwise {@code false}
     */
    @NativeCall
    boolean hasJavascriptProperty(JavascriptContext context, JavascriptObject object, String propertyName);
}
