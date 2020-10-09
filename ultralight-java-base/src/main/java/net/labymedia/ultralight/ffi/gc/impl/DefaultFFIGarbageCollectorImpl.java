/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package net.labymedia.ultralight.ffi.gc.impl;

import net.labymedia.ultralight.ffi.gc.BoundDeleter;
import net.labymedia.ultralight.ffi.gc.DeletableObject;
import net.labymedia.ultralight.ffi.gc.FFIGarbageCollector;
import net.labymedia.ultralight.util.Util;

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

                deleter.delete();
            } catch (InterruptedException ignored) {
            }
        }
    }
}
