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

import com.labymedia.ultralight.annotation.NativeType;

/**
 * A global JavaScript execution context. A {@link JavascriptGlobalContext} is a {@link JavascriptContext}.
 */
@NativeType("JSGlobalContextRef")
public class JavascriptGlobalContext extends JavascriptContext {
    /**
     * Constructs a new {@link JavascriptGlobalContext} wrapping the given native handle.
     *
     * @param handle The native handle to wrap
     */
    private JavascriptGlobalContext(@NativeType("JSGlobalContextRef") long handle, JavascriptContextLock lock) {
        super(handle, lock);
        lock.addDependency(this);
    }

    /**
     * Retrieves the name of the context. A {@link JavascriptGlobalContext}s name is exposed for remote debugging to
     * make it easier to identify the context you would like to attach to.
     *
     * @return The name of the context
     */
    public native String getName();

    /**
     * Sets the remote debugging name for this context.
     *
     * @param name The remote debugging name to set on this context
     */
    public native void setName(String name);

    /**
     * Releases the reference to this context.
     */
    @Override
    public native void contextUnlocking();
}
