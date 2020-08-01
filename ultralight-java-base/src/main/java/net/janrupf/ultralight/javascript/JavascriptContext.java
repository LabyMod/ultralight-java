package net.janrupf.ultralight.javascript;

import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.ffi.ObjectWithHandle;

import java.lang.annotation.Native;

/**
 * A JavaScript execution context. Holds the global object and other execution state.
 */
@NativeType("JSContextRef")
public class JavascriptContext implements ObjectWithHandle, JavascriptLockedObject {
    private final long handle;
    private final JavascriptContextLock lock;

    /**
     * Constructs a new {@link JavascriptContext} wrapping the given native handle.
     *
     * @param handle The native handle to this context
     * @param lock The lock this context belongs to
     */
    protected JavascriptContext(@NativeType("JSGlobalContextRef") long handle, JavascriptContextLock lock) {
        this.handle = handle;
        this.lock = lock;
    }

    /**
     * Retrieves the global context for this context.
     *
     * @return The global context owning this context
     */
    public native JavascriptGlobalContext getGlobalContext();

    @Override
    public void contextUnlocking() {}

    @Override
    public JavascriptContextLock getLock() {
        return lock;
    }

    @Override
    public long getContextHandle() {
        return lock.getContext().getHandle();
    }

    @Override
    public long getHandle() {
        if(!lock.isLocked()) {
            throw new IllegalStateException("JavascriptContext is not locked anymore");
        }

        return handle;
    }
}
