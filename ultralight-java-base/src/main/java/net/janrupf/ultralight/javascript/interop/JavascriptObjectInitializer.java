package net.janrupf.ultralight.javascript.interop;

import net.janrupf.ultralight.annotation.NativeCall;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.javascript.JavascriptContext;
import net.janrupf.ultralight.javascript.JavascriptObject;

/**
 * Callback for the initialization of Javascript objects.
 */
@NativeType("JSObjectInitializeCallback")
public interface JavascriptObjectInitializer {
    /**
     * The callback invoked when an object is first created.
     * <p>
     * Unlike the other object callbacks, the initialize callback is called on the least
     * derived class (the parent class) first, and the most derived class last.
     * <p>
     * <b>The parameters are only valid for the duration of execution of this method!</b>
     *
     * @param context The execution context to use
     * @param object The JSObject being created
     */
    @NativeCall
    void initializeJavascriptObject(JavascriptContext context, JavascriptObject object);
}
