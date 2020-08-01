package net.janrupf.ultralight.javascript.interop;

import net.janrupf.ultralight.annotation.NativeCall;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.javascript.JavascriptContext;
import net.janrupf.ultralight.javascript.JavascriptObject;
import net.janrupf.ultralight.javascript.JavascriptValue;

/**
 * Callback for calling a Javascript object with the new operator.
 */
@NativeType("JSObjectCallAsConstructorCallback")
public interface JavascriptObjectConstructor {
    /**
     * The callback invoked when an object is used as a constructor in a 'new' expression.
     *
     * @param context     The execution context to use
     * @param constructor An object that is the constructor being called
     * @param arguments   An array of the  arguments passed to the function
     * @return An object that is the constructor's return value
     * @throws JavascriptInteropException If an error occurs while invoking the constructor
     */
    @NativeCall
    JavascriptObject callAsJavascriptConstructor(
            JavascriptContext context, JavascriptObject constructor, JavascriptValue[] arguments)
            throws JavascriptInteropException;
}
