package net.janrupf.ultralight.javascript.interop;

import net.janrupf.ultralight.annotation.NativeCall;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.javascript.JavascriptContext;
import net.janrupf.ultralight.javascript.JavascriptObject;
import net.janrupf.ultralight.javascript.JavascriptType;
import net.janrupf.ultralight.javascript.JavascriptValue;

/**
 * Callback for converting a Javascript object to another type.
 */
@NativeType("JSObjectConvertToTypeCallback")
public interface JavascriptObjectToTypeConverter {
    /**
     * The callback invoked when converting an object to a particular JavaScript type.
     *
     * @param context The execution context to use
     * @param object  The object to convert
     * @param type    A {@link JavascriptType} specifying the JavaScript type to convert to
     * @return The object's converted value, must not be null
     * @throws JavascriptInteropException If an error occurs while converting the object
     */
    @NativeCall
    JavascriptValue convertToJavascriptType(JavascriptContext context, JavascriptObject object, JavascriptType type)
            throws JavascriptInteropException;
}
