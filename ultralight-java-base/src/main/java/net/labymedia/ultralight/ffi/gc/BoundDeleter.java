package net.labymedia.ultralight.ffi.gc;

/**
 * Bound version of a {@link ObjectDeleter}.
 */
public class BoundDeleter<T> {
    private final T value;
    private final ObjectDeleter<T> deleter;

    private boolean deleted;

    /**
     * Crates a new {@link BoundDeleter} binding an object and its deleter.
     *
     * @param value   The object to bind to the deleter
     * @param deleter The deleter to bind the object to
     */
    BoundDeleter(T value, ObjectDeleter<T> deleter) {
        this.value = value;
        this.deleter = deleter;
        this.deleted = false;
    }

    /**
     * Runs the deleter with the stored object.
     *
     * @return {@code true} if the object has been deleted, {@code false} otherwise
     */
    public boolean delete() {
        if(deleted) {
            return false;
        }

        deleter.delete(value);
        deleted = true;
        return true;
    }

    /**
     * Determines if this deleter has been executed already.
     *
     * @return {@code true} if the deleter has been executed already, {@code false} otherwise
     */
    public boolean isDeleted() {
        return deleted;
    }

    @Override
    public String toString() {
        return "BoundDeleter{" +
                "value=" + value +
                ", deleter=" + deleter +
                '}';
    }
}
