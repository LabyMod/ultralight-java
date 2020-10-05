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

package net.labymedia.ultralight.javascript;

import net.labymedia.ultralight.annotation.NativeCall;
import net.labymedia.ultralight.annotation.NativeType;
import net.labymedia.ultralight.annotation.Unsigned;

/**
 * Interface for objects depending on the context lock. Useful for invalidation when the context is unlocked.
 */
public interface JavascriptLockedObject {
    /**
     * Called when the lock is about to be released. This can be used to release the internal references. After this
     * call returned the object must not use the context anymore.
     */
    void contextUnlocking();

    /**
     * Retrieves the lock the object is currently locked with.
     *
     * @return The lock the object is currently locked with
     */
    JavascriptContextLock getLock();

    /**
     * Retrieves the handle of the context this object is currently locked with. If the object is not locked anymore,
     * this method throws an {@link IllegalStateException}.
     *
     * @return The handle of the context this object is currently locked with
     * @throws IllegalStateException If the context object is not locked anymore
     */
    @NativeCall
    @Unsigned
    @NativeType("JSContextRef")
    long getContextHandle();

    /**
     * Retrieves the handle of the lock this object is locked with. If the object is not locked anymore, this method
     * throws an {@link IllegalStateException}.
     *
     * @return The handle of the lock this object is currently locked with
     * @throws IllegalStateException If the context object is not locked anymore
     */
    @NativeCall
    @Unsigned
    @NativeType("ultralight_java::HoldContextLock *")
    long getLockHandle();
}
