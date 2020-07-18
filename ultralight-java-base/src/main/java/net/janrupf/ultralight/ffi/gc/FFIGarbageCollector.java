package net.janrupf.ultralight.ffi.gc;

/**
 * Interface for abstracting away the FFI garbage collector to allow
 * users to implement their own.
 */
public interface FFIGarbageCollector {
    /**
     * Submits a new object the garbage collector should listen for deletion to.
     * No reference to the value may be kept inside the instance after this method returned.
     *
     * @param value The object to listen for deletion to
     * @param deleter The deleter to invoke when the object gets deleted
     * @param <T> The type of the object to be deleted
     */
    <T> void submit(DeletableObject<T> value, BoundDeleter<T> deleter);
}
