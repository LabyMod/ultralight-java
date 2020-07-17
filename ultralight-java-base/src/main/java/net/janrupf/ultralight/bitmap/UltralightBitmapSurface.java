package net.janrupf.ultralight.bitmap;

import net.janrupf.ultralight.UltralightSurface;
import net.janrupf.ultralight.UltralightView;

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
