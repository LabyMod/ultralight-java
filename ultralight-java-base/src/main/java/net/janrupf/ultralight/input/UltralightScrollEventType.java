package net.janrupf.ultralight.input;

import net.janrupf.ultralight.annotation.NativeType;

import java.lang.annotation.Native;

/**
 * The scroll event granularity type
 */
@NativeType("ultralight::ScrollEvent::Type")
public enum UltralightScrollEventType {
    /**
     * The delta value is interpreted as number of pixels
     */
    @Native
    BY_PIXEL,

    /**
     * The delta value is interpreted as number of pages
     */
    @Native
    BY_PAGE
}
