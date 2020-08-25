package net.labymedia.ultralight.javascript.interop;

import net.labymedia.ultralight.annotation.NativeCall;
import net.labymedia.ultralight.annotation.NativeType;
import net.labymedia.ultralight.javascript.JavascriptObject;

/**
 * Callback for the finalization of Javascript objects.
 */
@NativeType("JSObjectFinalizeCallback")
public interface JavascriptObjectFinalizer {
    /**
     * The callback invoked when an object is finalized (prepared for garbage collection). An object may be finalized on
     * any thread.
     * <p>
     * The finalize callback is called on the most derived class first, and the least derived class (the parent class)
     * last.
     * <p>
     * <b>Functions which may cause garbage collection or allocation of new objects must not be called from inside this
     * callback!</b>
     *
     * @param object The object being finalized
     */
    @NativeCall
    void finalizeJavascriptObject(JavascriptObject object);
}
