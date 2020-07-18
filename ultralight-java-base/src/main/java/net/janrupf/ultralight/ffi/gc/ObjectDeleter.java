package net.janrupf.ultralight.ffi.gc;

/**
 * Callback when an {@link DeletableObject} has been garbage collected.
 *
 * @param <T> The type of the collected object.
 */
public interface ObjectDeleter<T> {
    /**
     * Executes the deletion routine for the object.
     *
     * @param value The object to delete
     */
    void delete(T value);
}
