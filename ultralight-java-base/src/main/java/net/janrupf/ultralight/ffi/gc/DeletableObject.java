package net.janrupf.ultralight.ffi.gc;

/**
 * A java object which submits itself to a garbage collection queue to invoke deletion
 * operations.
 *
 * @param <T> The type of the object which will be deleted by an {@link ObjectDeleter}.
 */
public class DeletableObject<T> {
    private final T value;

    /**
     * Constructs a new {@link DeletableObject} with a {@link ObjectDeleter} which
     * will be notified when this object will be deleted.
     *
     * @param value The deletable value
     * @param deleter The deleter to invoke when deleting the value
     */
    public DeletableObject(T value, ObjectDeleter<T> deleter) {
        this.value = value;

        // Submit the new object to the garbage collector
        FFIGarbageCollectorController.submit(this, new BoundDeleter<>(value, deleter));
    }

    /**
     * Retrieves the stored value of this {@link DeletableObject}. The value needs to not be kept around
     * or heap corruption may occur when the original deletable object goes out of scope.
     *
     * @return The stored value
     */
    public T get() {
        return value;
    }
}
