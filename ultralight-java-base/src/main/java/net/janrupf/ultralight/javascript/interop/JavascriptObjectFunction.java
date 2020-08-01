package net.janrupf.ultralight.javascript.interop;

import net.janrupf.ultralight.annotation.NativeCall;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.javascript.JavascriptContext;
import net.janrupf.ultralight.javascript.JavascriptObject;
import net.janrupf.ultralight.javascript.JavascriptValue;

/**
 * Callback for invoking a Javascript object as a function.
 */
@NativeType("JSObjectCallAsFunctionCallback")
public interface JavascriptObjectFunction {
    /**
     * The callback invoked when an object is called as a function.
     *
     * @param context    The execution context to use
     * @param function   An object that is the function being called
     * @param thisObject An object that is the 'this' variable in the function's scope
     * @param arguments  An array of the  arguments passed to the function
     * @return The return value of the function, must not be null
     * @throws JavascriptInteropException If an error occurs while invoking the function
     */
    @NativeCall
    JavascriptValue callAsJavascriptFunction(
            JavascriptContext context,
            JavascriptObject function,
            JavascriptObject thisObject,
            JavascriptValue[] arguments
    ) throws JavascriptInteropException;
}
