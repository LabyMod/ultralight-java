/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 - 2021 LabyMedia and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

package com.labymedia.ultralight.javascript;

import com.labymedia.ultralight.annotation.NativeCall;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.ffi.ObjectWithHandle;

import java.util.ArrayList;
import java.util.List;

/**
 * Lock hold on the JavascriptContext for safe execution.
 * <b>Not releasing (closing) the lock will block execution of Javascript forever!</b>
 */
@NativeType("ultralight::Ref<ultralight::JSContext>")
public class JavascriptContextLock implements AutoCloseable, ObjectWithHandle {
    private final long handle;
    private final List<JavascriptLockedObject> lockedObjects;
    private boolean isLocked;

    /**
     * Constructs a new {@link JavascriptContextLock} wrapping the given native lock.
     *
     * @param handle A handle to the native lock to wrap
     */
    @NativeCall
    private JavascriptContextLock(long handle) {
        this.handle = handle;
        this.lockedObjects = new ArrayList<>();

        this.isLocked = true;
    }

    /**
     * Determines whether this lock is still active.
     *
     * @return {@code true} if this lock is still active, {@code false} otherwise
     */
    public boolean isLocked() {
        return isLocked;
    }

    /**
     * Adds an object which has been locked using this context to the list of locked objects.
     *
     * @param object The object to add
     */
    public void addDependency(JavascriptLockedObject object) {
        this.lockedObjects.add(object);
    }

    @Override
    public void close() {
        unlock();
    }

    /**
     * Releases the lock if it has not been released already.
     */
    public void unlock() {
        if (isLocked) {
            // Notify all objects that the lock is being closed
            for (JavascriptLockedObject object : lockedObjects) {
                object.contextUnlocking();
            }

            lockedObjects.clear();

            release();
            isLocked = false;
        }
    }

    @Override
    public long getHandle() {
        if (!isLocked) {
            throw new IllegalStateException("JavascriptContextLock is not locked anymore");
        }

        return handle;
    }

    /**
     * Retrieves the context this lock has acquired.
     *
     * @return The context this lock has acquired
     */
    public native JavascriptContext getContext();

    /**
     * Releases this lock.
     */
    private native void release();
}
