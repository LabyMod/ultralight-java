package net.labymedia.ultralight.javascript.interop;

import net.labymedia.ultralight.annotation.NativeCall;
import net.labymedia.ultralight.annotation.NativeType;
import net.labymedia.ultralight.javascript.JavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptObject;
import net.labymedia.ultralight.javascript.JavascriptValue;

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
