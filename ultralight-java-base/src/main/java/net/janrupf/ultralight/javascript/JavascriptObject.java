package net.janrupf.ultralight.javascript;

public class JavascriptObject extends JavascriptValue {
    /**
     * Constructs a new {@link JavascriptObject} wrapping an existing value reference in a locked context.
     *
     * @param handle The native value reference
     * @param lock   The context the value exists in
     */
    JavascriptObject(long handle, JavascriptContextLock lock) {
        super(handle, lock);
    }
}
