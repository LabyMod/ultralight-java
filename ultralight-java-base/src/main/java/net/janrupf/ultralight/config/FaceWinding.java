package net.janrupf.ultralight.config;

import net.janrupf.ultralight.annotation.NativeType;

import java.lang.annotation.Native;

/**
 * The winding order for front-facing triangles.
 * <p>
 * In most 3D engines, there is the concept that triangles have a
 * a "front" and a "back". All the front-facing triangles (eg, those
 * that are facing the camera) are rendered, and all back-facing
 * triangles are culled (ignored). The winding-order of the triangle's
 * vertices is used to determine which side is front and back. You
 * should tell Ultralight which winding-order your 3D engine uses.
 */
@NativeType("ultralight::FaceWinding")
public enum FaceWinding {
    /**
     * Clockwise Winding (Direct3D, etc.)
     */
    @Native
    CLOCKWISE,

    /**
     * Counter-Clockwise Winding (OpenGL, etc.)
     */
    @Native
    COUNTER_CLOCKWISE
}
