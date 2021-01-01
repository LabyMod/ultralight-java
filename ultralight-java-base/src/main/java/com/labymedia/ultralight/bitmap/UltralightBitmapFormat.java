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

import com.labymedia.ultralight.annotation.NativeType;

import java.lang.annotation.Native;

/**
 * The various Bitmap formats.
 */
@NativeType("ultralight::BitmapFormat")
public enum UltralightBitmapFormat {
    /**
     * Alpha channel only, 8-bits per pixel.
     * <p>
     * Encoding: 8-bits per channel, unsigned normalized.
     * <p>
     * Color-space: Linear (no gamma), alpha-coverage only.
     */
    @Native
    A8_UNORM(1),

    /**
     * Blue Green Red Alpha channels, 32-bits per pixel.
     * <p>
     * Encoding: 8-bits per channel, unsigned normalized.
     * <p>
     * Color-space: sRGB gamma with premultiplied linear alpha channel.
     * <p>
     * <b>Alpha is premultiplied with BGR channels <i>before</i> sRGB gamma is
     * applied so we can use sRGB conversion hardware and perform all
     * blending in linear space on GPU.</b>
     */
    @Native
    BGRA8_UNORM_SRGB(4);

    private final int bytesPerPixel;

    UltralightBitmapFormat(int bytesPerPixel) {
        this.bytesPerPixel = bytesPerPixel;
    }

    /**
     * Retrieves the amount of bytes per pixel.
     *
     * @return The amount of bytes per pixel
     */
    public int getBytesPerPixel() {
        return bytesPerPixel;
    }
}
