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

package com.labymedia.ultralight.config;

import com.labymedia.ultralight.annotation.NativeType;

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
