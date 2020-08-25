package net.labymedia.ultralight.javascript.interop;

import net.labymedia.ultralight.annotation.NativeCall;
import net.labymedia.ultralight.annotation.NativeType;
import net.labymedia.ultralight.javascript.JavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptObject;
import net.labymedia.ultralight.javascript.JavascriptType;
import net.labymedia.ultralight.javascript.JavascriptValue;

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
