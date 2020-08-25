package net.labymedia.ultralight.javascript;

/**
 * A Javascript value that is currently not within any locked context. Those values keep references to their underlying
 * objects and prevent them from freeing. Don't keep these objects around forever, as this will create a memory leak
 * else.
 */
public class JavascriptProtectedValue {
    private long handle;

    /**
     * Constructs a new {@link JavascriptProtectedValue} wrapping the given native value reference. The native value
     * needs to be protected already!
     *
     * @param handle The native, already protected handle
     */
    JavascriptProtectedValue(long handle) {
        this.handle = handle;
    }

    /**
     * "revives" the value by attaching it to a lock again. This invalidates this value.
     *
     * @param lock The lock to attach the value to
     * @return The reattached value
     * @throws IllegalArgumentException If the given lock is null
     * @throws IllegalStateException    If the value has been revived already
     */
    public JavascriptValue revive(JavascriptContextLock lock) {
        if(lock == null) {
            throw new IllegalArgumentException(new NullPointerException("lock"));
        } else if(handle == 0) {
            throw new IllegalStateException("The value has been revived already");
        }

        long tHandle = handle;
        handle = 0;

        return new JavascriptValue(tHandle, lock);
    }
}
