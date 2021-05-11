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

package com.labymedia.ultralight;

import com.labymedia.ultralight.annotation.AntiFree;
import com.labymedia.ultralight.annotation.NativeCall;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.annotation.Unsigned;
import com.labymedia.ultralight.ffi.ObjectWithHandle;
import com.labymedia.ultralight.math.IntRect;

import java.nio.ByteBuffer;
import java.util.Objects;

/**
 * Offscreen pixel buffer surface. (Premultiplied BGRA 32-bit format).
 * <p>
 * Each View is rendered to a Surface (@see View::surface) when the GPU
 * renderer is disabled (the default).
 * <p>
 * The default Surface implementation is BitmapSurface, you can retrieve the
 * underlying bitmap via BitmapSurface::bitmap.
 * <p>
 * You can also provide your own Surface implementation via SurfaceFactory.
 * This can be used to wrap a platform-specific GPU texture, Windows DIB,
 * macOS CGImage, or any other pixel buffer target for display on screen.
 */
public class UltralightSurface implements ObjectWithHandle {
    @AntiFree
    private final UltralightView view;
    private final long handle;

    /**
     * Constructs a new {@link UltralightSurface} from an underlying handle.
     *
     * @param view   The view this surface belongs to, used to prevent the view from
     *               being freed to early
     * @param handle The underlying handle
     */
    @NativeCall
    protected UltralightSurface(@AntiFree UltralightView view, long handle) {
        this.view = view;
        this.handle = handle;
    }

    /**
     * Width (in pixels).
     *
     * @return The width
     */
    public native @NativeType("uint32_t")
    @Unsigned
    long width();

    /**
     * Height (in pixels).
     *
     * @return The height
     */
    public native @NativeType("uint32_t")
    @Unsigned
    long height();

    /**
     * Number of bytes between rows (usually width * 4).
     *
     * @return The number of bytes between rows
     */
    public native @NativeType("uint32_t")
    @Unsigned
    long rowBytes();

    /**
     * Size in bytes.
     *
     * @return The size in bytes
     */
    public native @NativeType("size_t")
    @Unsigned
    long size();

    /**
     * Lock the pixel buffer and get a pointer to the beginning of the data
     * for reading/writing.
     * <p>
     * Native pixel format is premultiplied BGRA 32-bit (8 bits per channel).
     *
     * @return A buffer containing the pixels
     * @see #unlockPixels()
     */
    public native @NativeType("void *")
    ByteBuffer lockPixels();

    /**
     * Unlock the pixel buffer.
     *
     * @see #lockPixels()
     */
    public native void unlockPixels();

    /**
     * Resize the pixel buffer to a certain width and height (both in pixels).
     * <p>
     * This should never be called while pixels are locked.
     *
     * @param width  The new width in pixels
     * @param height The new height in pixels
     */
    public native void resize(@NativeType("uint32_t") @Unsigned long width, @NativeType("uint32_t") @Unsigned long height);

    /**
     * Set the dirty bounds to a certain value.
     * This is called after the Renderer paints to an area of the pixel buffer.
     * (The new value will be joined with the existing dirty_bounds())
     *
     * @param rect The new dirty bounds
     */
    public native void setDirtyBounds(IntRect rect);

    /**
     * Get the dirty bounds.
     * <p>
     * This value can be used to determine which portion of the pixel buffer has
     * been updated since the last call to ClearDirtyBounds().
     * <p>
     * The general algorithm to determine if a Surface needs display is:
     * <pre>
     *     <code>
     * if (!surface.dirtyBounds().isEmpty()) {
     *     // Surface pixels are dirty and needs display.
     *     // Cast Surface to native Surface and use it here (pseudo code)
     *     displaySurface(surface);
     *     // Once you're done, clear the dirty bounds:
     *     surface.clearDirtyBounds();
     * }
     *      </code>
     *  </pre>
     *
     * @return The dirty bounds of this rectangle
     */
    public native IntRect dirtyBounds();

    /**
     * Clear the dirty bounds.
     * <p>
     * You should call this after you're done displaying the Surface.
     */
    public native void clearDirtyBounds();

    @Override
    public long getHandle() {
        return handle;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof UltralightSurface)) return false;
        UltralightSurface that = (UltralightSurface) o;

        // Only check the handle, the view can be another object
        return handle == that.handle;
    }

    @Override
    public int hashCode() {
        // Only hash the handle, the view can be another object
        return Objects.hash(handle);
    }
}
