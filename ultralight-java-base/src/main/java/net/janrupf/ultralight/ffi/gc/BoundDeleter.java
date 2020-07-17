package net.janrupf.ultralight.ffi.gc;

/**
 * Bound version of a {@link ObjectDeleter}.
 */
public class BoundDeleter<T> {
    private final T value;
    private final ObjectDeleter<T> deleter;

    /**
     * Crates a new {@link BoundDeleter} binding an object and its deleter.
     *
     * @param value The object to bind to the deleter
     * @param deleter The deleter to bind the object to
     */
    BoundDeleter(T value, ObjectDeleter<T> deleter) {
        this.value = value;
        this.deleter = deleter;
    }

    /**
     * Runs the deleter with the stored object.
     */
    public void delete() {
        deleter.delete(value);
    }
}
