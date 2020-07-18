package net.janrupf.ultralight.ffi.gc.impl;

import net.janrupf.ultralight.ffi.gc.BoundDeleter;
import net.janrupf.ultralight.ffi.gc.DeletableObject;
import net.janrupf.ultralight.ffi.gc.FFIGarbageCollector;
import net.janrupf.ultralight.ffi.gc.ObjectDeleter;
import net.janrupf.ultralight.util.Util;

import java.lang.ref.Reference;
import java.lang.ref.ReferenceQueue;
import java.lang.ref.WeakReference;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

/**
 * A default implementation of a {@link FFIGarbageCollector} using a {@link Thread}
 * and a {@link java.lang.ref.ReferenceQueue} for deletion.
 */
public class DefaultFFIGarbageCollectorImpl implements FFIGarbageCollector {
    private final ReferenceQueue<DeletableObject<?>> queue;
    private final Map<Reference<DeletableObject<?>>, BoundDeleter<?>> deleters;

    /**
     * Constructs a new {@link DefaultFFIGarbageCollectorImpl} and starts its internal garbage
     * collection thread.
     */
    public DefaultFFIGarbageCollectorImpl() {
        this.queue = new ReferenceQueue<>();
        this.deleters = new ConcurrentHashMap<>();

        // Create the thread and set it as a daemon, it will never terminate and constantly
        // ask the queue for references to delete
        Thread collectionThread = new Thread(this::run, "Ultralight garbage collector");
        collectionThread.setDaemon(true);
        collectionThread.start();
    }

    @Override
    public <T> void submit(DeletableObject<T> value, BoundDeleter<T> deleter) {
        Reference<DeletableObject<T>> reference = new WeakReference<>(value, queue);
        deleters.put(Util.forceCast(reference), deleter);
    }

    /**
     * Runs the loop of this garbage collector and begins
     * searching the queue for deletable objects. This method does not listen to
     * {@link InterruptedException} and should only be called from a daemon thread.
     */
    private void run() {
        while (true) {
            try {
                // Extract the next reference and its deleter
                Reference<? extends DeletableObject<?>> reference = queue.remove();
                BoundDeleter<?> deleter = deleters.remove(reference);

                if(deleter == null) {
                    // Hopefully no-op
                    throw new IllegalStateException(
                            "Object in reference queue which does not have a deleter associated");
                }

                // Execute the deleter
                deleter.delete();
            } catch (InterruptedException ignored) {
            }
        }
    }
}
