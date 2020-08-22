package net.janrupf.ultralight.bitmap;

import net.janrupf.ultralight.annotation.NativeCall;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.annotation.Unsigned;
import net.janrupf.ultralight.ffi.ObjectWithHandle;
import net.janrupf.ultralight.ffi.RefPtr;
import net.janrupf.ultralight.math.IntRect;

import java.nio.ByteBuffer;

/**
 * Bitmap container with basic blitting and conversion routines.
 */
@NativeType("ultralight::RefPtr<ultralight::Bitmap>")
public class UltralightBitmap implements ObjectWithHandle {
    /**
     * Create an empty Bitmap. No pixels will be allocated.
     *
     * @return The created bitmap
     */
    public static native UltralightBitmap create();

    /**
     * Create a Bitmap with a certain configuration. Pixels will be allocated
     * but not initialized.
     *
     * @param width  The width in pixels
     * @param height The height in pixels
     * @param format The pixel format to use
     * @return A ref-pointer to a new Bitmap instance
     */
    public static native UltralightBitmap create(
            @NativeType("uint32_t") @Unsigned long width,
            @NativeType("uint32_t") @Unsigned long height,
            UltralightBitmapFormat format
    );

    /**
     * Create a Bitmap with existing pixels and configuration.
     *
     * @param width      The width in pixels
     * @param height     The height in pixels
     * @param format     The pixel format to use
     * @param rowBytes   The number of bytes between each row (note that this
     *                   value should be &gt;= width * {@link UltralightBitmapFormat#getBytesPerPixel()})
     * @param pixels     Pointer to raw pixel buffer,
     * @param fixupGamma Whether or not we should reinterpret the source
     *                   as an sRGB bitmap with premultiplied alpha applied
     *                   after the gamma function (typical of PNGs). We
     *                   expect all premultiplication to be applied before
     *                   the gamma function so we can blend properly in
     *                   linear space. Only valid for {@link UltralightBitmapFormat#BGRA8_UNORM_SRGB}
     * @return A ref-pointer to a new Bitmap instance
     */
    public static native UltralightBitmap create(
            @NativeType("uint32_t") @Unsigned long width,
            @NativeType("uint32_t") @Unsigned long height,
            UltralightBitmapFormat format,
            @NativeType("uint32_t") @Unsigned long rowBytes,
            ByteBuffer pixels,
            boolean fixupGamma
    );

    /**
     * Create a Bitmap from a deep copy of another Bitmap.
     *
     * @param bitmap The Bitmap to copy
     * @return The copied Bitmap
     */
    public static native UltralightBitmap create(UltralightBitmap bitmap);

    private final RefPtr<UltralightBitmap> ptr;

    /**
     * Constructs a new {@link UltralightBitmap} from the given native
     * {@link RefPtr}.
     *
     * @param ptr The pointer managing the bitmap
     */
    @NativeCall
    private UltralightBitmap(RefPtr<UltralightBitmap> ptr) {
        this.ptr = ptr;
    }

    /**
     * Get the width in pixels.
     *
     * @return The width in pixels
     */
    public native @NativeType("uint32_t") @Unsigned long width();

    /**
     * Get the height in pixels.
     *
     * @return The height in pixels
     */
    public native @NativeType("uint32_t") @Unsigned long height();

    /**
     * Get the bounds as an {@link IntRect}.
     *
     * @return The bounds as a rectangle
     */
    public native IntRect bounds();

    /**
     * Get the pixel format.
     *
     * @return The pixel format
     */
    public native UltralightBitmapFormat format();

    /**
     * Get the number of bytes per pixel.
     *
     * @return The number of bytes per pixel
     */
    public native @NativeType("uint32_t") @Unsigned long bpp();

    /**
     * Get the number of bytes between each row (this is always &gt;= width * {@link #bpp()}).
     *
     * @return The number of bytes between each row
     */
    public native @NativeType("uint32_t") @Unsigned long rowBytes();

    /**
     * Get the size in bytes of the pixel buffer.
     *
     * @return The size in bytes of the pixel buffer
     */
    public native @NativeType("size_t") @Unsigned long size();

    /**
     * Whether or not this Bitmap owns the pixel buffer and will destroy it
     * at the end of its lifetime.
     *
     * @return Whether this bitmap owns the pixel buffer
     */
    public native boolean ownsPixels();

    /**
     * Lock the pixel buffer for reading/writing.
     *
     * @return The pixel buffer
     */
    public native ByteBuffer lockPixels();

    /**
     * Unlock the pixel buffer.
     */
    public native void unlockPixels();

    /**
     * Get the raw pixel buffer.
     * <b>You should only call this if pixels are already locked!</b>
     *
     * @return The pixel buffer
     */
    public native ByteBuffer rawPixels();

    /**
     * Whether or not this Bitmap is empty (no pixels allocated).
     *
     * @return Whether this Bitmap is empty
     */
    public native boolean isEmpty();

    /**
     * Erase the Bitmap (set all pixels to 0).
     */
    public native void erase();

    /**
     * Assign another bitmap to this one.
     *
     * @param bitmap The bitmap to copy from
     */
    public native void set(UltralightBitmap bitmap);

    /**
     * Draw another bitmap to this bitmap.
     * <p>
     * Formats do not need to match. Bitmap formats will be converted
     * to one another automatically. Note that when converting from
     * BGRA8 to A8, only the Blue channel will be used.
     *
     * @param srcRect   The source rectangle, relative to src bitmap.
     * @param destRect  The destination rectangle, relative to this bitmap.
     * @param src       The source bitmap.
     * @param padRepeat Whether or not we should pad the drawn bitmap by one
     *                  pixel of repeated edge pixels from the source bitmap.
     * @return Whether or not the operation succeeded (this can fail if the
     * srcRect and/or destRect are invalid, or if their total
     * dimensions do not match).
     */
    public native boolean drawBitmap(IntRect srcRect, IntRect destRect, UltralightBitmap src, boolean padRepeat);

    /**
     * Write this Bitmap out to a PNG image. (mainly used for Debug).
     *
     * @param path The filepath to write to (opened with fopen())
     * @return Whether or not the operation succeeded.
     */
    public native boolean writePNG(String path);

    /**
     * Make a resized copy of this bitmap by writing to a pre-allocated
     * destination bitmap.
     *
     * @param destination The bitmap to store the result in, the width and
     *                    height of the destination will be used
     * @param highQuality Whether or not a high quality resampling will be
     *                    used during the resize. (Otherwise, just uses fast
     *                    nearest-neighbor sampling)
     * @return Whether or not the operation succeeded. This operation is only
     * valid if both formats are kBitmapFormat_BGRA8_UNORM_SRGB and
     * both the source and destination are non-empty
     */
    public native boolean resample(UltralightBitmap destination, boolean highQuality);

    /**
     * his converts a BGRA bitmap to RGBA bitmap and vice-versa by swapping
     * he red and blue channels.
     */
    public native void swapRedBlueChannels();

    @Override
    public long getHandle() {
        return ptr.getHandle();
    }
}
