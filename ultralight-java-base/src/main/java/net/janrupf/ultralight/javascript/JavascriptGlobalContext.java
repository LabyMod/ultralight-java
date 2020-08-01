package net.janrupf.ultralight.javascript;

import net.janrupf.ultralight.annotation.NativeType;

/**
 * A global JavaScript execution context. A {@link JavascriptGlobalContext} is a {@link JavascriptContext}.
 */
@NativeType("JSGlobalContextRef")
public class JavascriptGlobalContext extends JavascriptContext {
    /**
     * Constructs a new {@link JavascriptGlobalContext} wrapping the given native handle.
     *
     * @param handle The native handle to wrap
     */
    private JavascriptGlobalContext(@NativeType("JSGlobalContextRef") long handle, JavascriptContextLock lock) {
        super(handle, lock);
        lock.addDependency(this);
    }

    /**
     * Retrieves the name of the context. A {@link JavascriptGlobalContext}s name is exposed for remote debugging to
     * make it easier to identify the context you would like to attach to.
     *
     * @return The name of the context
     */
    public native String getName();

    /**
     * Sets the remote debugging name for this context.
     *
     * @param name The remote debugging name to set on this context
     */
    public native void setName(String name);

    /**
     * Releases the reference to this context.
     */
    @Override
    public native void contextUnlocking();
}
