package net.labymedia.ultralight.config;

import net.labymedia.ultralight.annotation.NativeType;

import java.lang.annotation.Native;

/**
 * Representation of the native enum `ultralight::FontHinting`
 */
@NativeType("ultralight::FontHinting")
public enum FontHinting {
    /**
     * Lighter hinting algorithm-- glyphs are slightly fuzzier but better
     * resemble their original shape. This is achieved by snapping glyphs to the
     * pixel grid only vertically which better preserves inter-glyph spacing.
     */
    @Native
    SMOOTH,

    /**
     * Default hinting algorithm-- offers a good balance between sharpness and
     * shape at smaller font sizes.
     */
    @Native
    NORMAL,

    /**
     * Strongest hinting algorithm-- outputs only black/white glyphs. The result
     * is usually unpleasant if the underlying TTF does not contain hints for
     * this type of rendering.
     */
    @Native
    MONOCHROME
}
