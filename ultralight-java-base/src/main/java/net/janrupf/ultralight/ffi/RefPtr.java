package net.janrupf.ultralight.ffi;

import net.janrupf.ultralight.annotation.NativeCall;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.annotation.Unsigned;
import net.janrupf.ultralight.ffi.gc.DeletableObject;

/**
 * Wrapper for Ultralight smart pointers. Under the hood this RefPtr will only ever have one reference
 * on the java side, regardless of how many times it is copied. The underlying FFI garbage collector is
 * used to count these objects internally.
 *
 * @param <T> The type this pointer contains
 */
@NativeType("ultralight::RefPtr")
public class RefPtr<T> implements ObjectWithHandle {
    private final DeletableObject<Long> handle;

    /**
     * Constructs a new {@link RefPtr} and begins tracking the underlying handle.
     *
     * @param handle A pointer to a native `ultralight::RefPtr`, the java object will take
     *               ownership of the native pointer
     */
    @NativeCall
    private RefPtr(@NativeType("ultralight::RefPtr *") @Unsigned long handle) {
        this.handle = new DeletableObject<>(handle, RefPtr::delete);
    }

    @Override
    public @NativeType("ultralight::RefPtr *") @Unsigned long getHandle() {
        return handle.get();
    }

    /**
     * Executes the deletion of the native `ultralight::RefPtr` instance.
     * This method is static to not keep a reference to the java object, which
     * else would prevent deletion.
     *
     * @param handle A pointer to the instance to delete
     */
    private static native void delete(long handle);
}
