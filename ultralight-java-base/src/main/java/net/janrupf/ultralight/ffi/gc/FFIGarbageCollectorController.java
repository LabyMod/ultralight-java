package net.janrupf.ultralight.ffi.gc;

import net.janrupf.ultralight.ffi.gc.impl.DefaultFFIGarbageCollectorImpl;

/**
 * Garbage collector manager for FFI objects. Used in conjunction with
 * {@link DeletableObject}s.
 */
public class FFIGarbageCollectorController {
    private static FFIGarbageCollector collector;

    /**
     * Submits a new object for deletion with a given deleter.
     *
     * @param object The object to listen to deletion for
     * @param deleter The deleter to run when the object gets garbage collected
     * @param <T> The type to garbage collect
     */
    public static <T> void submit(DeletableObject<T> object, BoundDeleter<T> deleter) {
        if(collector == null) {
            // The user has not overwritten the collector
            collector = new DefaultFFIGarbageCollectorImpl();
        }

        collector.submit(object, deleter);
    }

    /**
     * Sets the global garbage collector instance used for tracking and deleting
     * FFI objects.
     * <p>
     * This may only be done if the collector has not been set already. The implementation
     * sets the default collector automatically when the first FFI garbage collected object
     * is created and no collector has been supplied by the user.
     *
     * @param collector The new garbage collector instance
     */
    public static void setCollector(FFIGarbageCollector collector) {
        if(FFIGarbageCollectorController.collector != null) {
            throw new IllegalStateException("The garbage collector has been set already, " +
                    "try setting it earlier if this has not been done by you");
        }

        FFIGarbageCollectorController.collector = collector;
    }
}
