package net.labymedia.ultralight.javascript;

import net.labymedia.ultralight.annotation.NativeCall;
import net.labymedia.ultralight.annotation.NativeType;
import net.labymedia.ultralight.ffi.ObjectWithHandle;

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
        if(isLocked) {
            // Notify all objects that the lock is being closed
            for(JavascriptLockedObject object : lockedObjects) {
                object.contextUnlocking();
            }

            lockedObjects.clear();

            release();
            isLocked = false;
        }
    }

    @Override
    public long getHandle() {
        if(!isLocked) {
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
