package net.janrupf.ultralight.javascript;

import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.ffi.ObjectWithHandle;
import net.janrupf.ultralight.ffi.gc.DeletableObject;

/**
 * A JavaScript class. Used with {@link JavascriptContext#makeObject(JavascriptClass, Object)}
 * to construct objects with custom behavior.
 */
@NativeType("JSClassRef")
public class JavascriptClass implements ObjectWithHandle {
    private final DeletableObject<Long> handle;

    /**
     * Constructs a new {@link JavascriptClass} wrapping an existing native handle.
     *
     * @param handle The native handle to wrap
     */
    private JavascriptClass(long handle) {
        this.handle = new DeletableObject<>(handle, JavascriptClass::release);
    }

    /**
     * Releases the class by decrementing its reference count.
     *
     * @param handle The native handle to decrement the reference count of
     */
    private static native void release(long handle);

    @Override
    public long getHandle() {
        return handle.get();
    }
}
