package net.labymedia.ultralight.ffi;

import net.labymedia.ultralight.annotation.NativeCall;
import net.labymedia.ultralight.annotation.NativeType;
import net.labymedia.ultralight.annotation.Unsigned;

/**
 * Interface used by native code to retrieve underlying pointers
 * from java instances,
 */
public interface ObjectWithHandle {
    /**
     * Retrieves the underlying native pointer from this instance.
     *
     * @return The underlying native pointer
     */
    @NativeCall
    @NativeType("void *")
    @Unsigned
    long getHandle();
}
