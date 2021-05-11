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

package com.labymedia.ultralight.bitmap;

import com.labymedia.ultralight.UltralightSurface;
import com.labymedia.ultralight.UltralightView;

/**
 * The default Surface implementation, backed by a Bitmap.
 */
public class UltralightBitmapSurface extends UltralightSurface {
    /**
     * Constructs a new {@link UltralightBitmapSurface} from an underlying handle.
     *
     * @param view   The view this surface belongs to, used to prevent the view from
     *               being freed to early
     * @param handle The underlying handle
     */
    protected UltralightBitmapSurface(UltralightView view, long handle) {
        super(view, handle);
    }

    /**
     * Get the underlying Bitmap.
     *
     * @return Retrieves the underlying Bitmap
     */
    public native UltralightBitmap bitmap();
}
