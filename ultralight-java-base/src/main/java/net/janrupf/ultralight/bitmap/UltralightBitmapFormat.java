package net.janrupf.ultralight.bitmap;

import net.janrupf.ultralight.annotation.NativeType;

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
