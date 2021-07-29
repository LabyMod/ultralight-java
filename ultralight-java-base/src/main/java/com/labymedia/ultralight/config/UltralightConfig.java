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

import com.labymedia.ultralight.UltralightPlatform;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.annotation.Unsigned;

import java.lang.annotation.Native;

/**
 * Configuration settings for Ultralight.
 * This is intended to be implemented by users and defined before creating the
 * Renderer.
 *
 * @see UltralightPlatform#setConfig(UltralightConfig)
 */
@NativeType("ultralight::Config")
public class UltralightConfig {
    /**
     * The file path to a writable directory that will be used to store cookies,
     * cached resources, and other persistent data.
     */
    @NativeType("ultralight::String16")
    @Native
    private String cachePath;

    /**
     * The winding order for front-facing triangles. @see FaceWinding
     * <p>
     * Note: This is only used when the GPU renderer is enabled.
     */
    @Native
    private FaceWinding faceWinding = FaceWinding.COUNTER_CLOCKWISE;

    /**
     * The hinting algorithm to use when rendering fonts.
     *
     * @see FontHinting
     */
    @Native
    private FontHinting fontHinting = FontHinting.NORMAL;

    /**
     * The gamma to use when compositing font glyphs, change this value to
     * adjust contrast (Adobe and Apple prefer 1.8, others may prefer 2.2).
     */
    @Native
    private double fontGamma = 1.8;

    /**
     * Default user stylesheet. You should set this to your own custom CSS
     * string to define default styles for various DOM elements, scrollbars,
     * and platform input widgets.
     */
    @NativeType("ultralight::String16")
    @Native
    private String userStylesheet = "";

    /**
     * Whether or not we should continuously repaint any Views or compositor
     * layers, regardless if they are dirty or not. This is mainly used to
     * diagnose painting/shader issues.
     */
    @Native
    private boolean forceRepaint = false;

    /**
     * When a CSS animation is active, the amount of time (in seconds) to wait
     * before triggering another repaint. Default is 60 Hz.
     */
    @Native
    private double animationTimerDelay = 1.0 / 60.0;

    /**
     * When a smooth scroll animation is active, the amount of time (in seconds)
     * to wait before triggering another repaint. Default is 60 Hz.
     */
    @Native
    private double scrollTimerDelay = 1.0 / 60.0;

    /**
     * The amount of time (in seconds) to wait before running the recycler (will
     * attempt to return excess memory back to the system).
     */
    @Native
    private double recycleDelay = 4.0;

    /**
     * Size of WebCore's memory cache in bytes.
     * <p>
     * You should increase this if you anticipate handling pages with
     * large resources, Safari typically uses 128+ MiB for its cache.
     */
    @NativeType("uint32_t")
    @Unsigned
    @Native
    private long memoryCacheSize = 64 * 1024 * 1024;

    /**
     * Number of pages to keep in the cache. Defaults to 0 (none).
     * <p>
     * Safari typically caches about 5 pages and maintains an on-disk
     * cache to support typical web-browsing activities. If you increase
     * this, you should probably increase the memory cache size as well.
     */
    @NativeType("uint32_t")
    @Unsigned
    @Native
    private long pageCacheSize = 0;

    /**
     * JavaScriptCore tries to detect the system's physical RAM size to set
     * reasonable allocation limits. Set this to anything other than 0 to
     * override the detected value. Size is in bytes.
     * <p>
     * This can be used to force JavaScriptCore to be more conservative with
     * its allocation strategy (at the cost of some performance).
     */
    @NativeType("uint32_t")
    @Unsigned
    @Native
    private long overrideRamSize = 0;

    /**
     * The minimum size of large VM heaps in JavaScriptCore. Set this to a
     * lower value to make these heaps start with a smaller initial value.
     */
    @NativeType("uint32_t")
    @Unsigned
    @Native
    private long minLargeHeapSize = 32 * 1024 * 1024;

    /**
     * The minimum size of small VM heaps in JavaScriptCore. Set this to a
     * lower value to make these heaps start with a smaller initial value.
     */
    @NativeType("uint32_t")
    @Unsigned
    @Native
    private long minSmallHeapSize = 1024 * 1024;

    /**
     * @deprecated This does nothing anymore
     */
    @Deprecated
    public UltralightConfig resourcePath(@SuppressWarnings("unused") String unused) {
        return this;
    }

    /**
     * Sets the cachePath field of this instance.
     *
     * @param cachePath The new value of the field
     * @return this
     * @see #cachePath
     */
    public UltralightConfig cachePath(String cachePath) {
        this.cachePath = cachePath;
        return this;
    }

    /**
     * Sets the faceWinding field of this instance.
     *
     * @param faceWinding The new value of the field
     * @return this
     * @see #faceWinding
     */
    public UltralightConfig faceWinding(FaceWinding faceWinding) {
        this.faceWinding = faceWinding;
        return this;
    }

    /**
     * Sets the fontHinting field of this instance.
     *
     * @param fontHinting The new value of the field
     * @return this
     * @see #fontHinting
     */
    public UltralightConfig fontHinting(FontHinting fontHinting) {
        this.fontHinting = fontHinting;
        return this;
    }

    /**
     * Sets the fontGamma field of this instance.
     *
     * @param fontGamma The new value of the field
     * @return this
     * @see #fontGamma
     */
    public UltralightConfig fontGamma(double fontGamma) {
        this.fontGamma = fontGamma;
        return this;
    }

    /**
     * Sets the userStylesheet field of this instance.
     *
     * @param userStylesheet The new value of the field
     * @return this
     * @see #userStylesheet
     */
    public UltralightConfig userStylesheet(String userStylesheet) {
        this.userStylesheet = userStylesheet;
        return this;
    }

    /**
     * Sets the forceRepaint field of this instance.
     *
     * @param forceRepaint The new value of the field
     * @return this
     * @see #forceRepaint
     */
    public UltralightConfig forceRepaint(boolean forceRepaint) {
        this.forceRepaint = forceRepaint;
        return this;
    }

    /**
     * Sets the animationDelayTimer field of this instance.
     *
     * @param animationTimerDelay The new value of the field
     * @return this
     * @see #animationTimerDelay
     */
    public UltralightConfig animationTimerDelay(double animationTimerDelay) {
        this.animationTimerDelay = animationTimerDelay;
        return this;
    }

    /**
     * Sets the scrollTimerDelay field of this instance.
     *
     * @param scrollTimerDelay The new value of the field
     * @return this
     * @see #scrollTimerDelay
     */
    public UltralightConfig scrollTimerDelay(double scrollTimerDelay) {
        this.scrollTimerDelay = scrollTimerDelay;
        return this;
    }

    /**
     * Sets the recycleDelay field of this instance.
     *
     * @param recycleDelay The new value of the field
     * @return this
     * @see #recycleDelay
     */
    public UltralightConfig recycleDelay(double recycleDelay) {
        this.recycleDelay = recycleDelay;
        return this;
    }

    /**
     * Sets the memoryCacheSize field of this instance.
     *
     * @param memoryCacheSize The new value of the field
     * @return this
     * @throws IllegalArgumentException If the parameter is negative
     * @see #memoryCacheSize
     */
    public UltralightConfig memoryCacheSize(long memoryCacheSize) {
        if (pageCacheSize < 0) {
            throw new IllegalArgumentException("memoryCacheSize can't be less than 0");
        }

        this.memoryCacheSize = memoryCacheSize;
        return this;
    }

    /**
     * Sets the pageCacheSize field of this instance.
     *
     * @param pageCacheSize The new value of the field
     * @return this
     * @throws IllegalArgumentException If the parameter is negative
     * @see #pageCacheSize
     */
    public UltralightConfig pageCacheSize(long pageCacheSize) {
        if (pageCacheSize < 0) {
            throw new IllegalArgumentException("pageCacheSize can't be less than 0");
        }

        this.pageCacheSize = pageCacheSize;
        return this;
    }

    /**
     * Sets the overrideRamSize field of this instance.
     *
     * @param overrideRamSize The new value of the field
     * @return this
     * @throws IllegalArgumentException If the parameter is negative
     * @see #overrideRamSize
     */
    public UltralightConfig overrideRamSize(long overrideRamSize) {
        if (overrideRamSize < 0) {
            throw new IllegalArgumentException("overrideRamSize can't be less than 0");
        }

        this.overrideRamSize = overrideRamSize;
        return this;
    }

    /**
     * Sets the minLargeHeapSize field of this instance.
     *
     * @param minLargeHeapSize The new value of the field
     * @return this
     * @throws IllegalArgumentException If the parameter is negative
     * @see #minLargeHeapSize
     */
    public UltralightConfig minLargeHeapSize(long minLargeHeapSize) {
        if (minLargeHeapSize < 0) {
            throw new IllegalArgumentException("minLargeHeapSize can't be less than 0");
        }

        this.minLargeHeapSize = minLargeHeapSize;
        return this;
    }

    /**
     * Sets the minSmallHeapSize field of this instance.
     *
     * @param minSmallHeapSize The new value of the field
     * @return this
     * @throws IllegalArgumentException If the parameter is negative
     * @see #minSmallHeapSize
     */
    public UltralightConfig minSmallHeapSize(long minSmallHeapSize) {
        if (minSmallHeapSize < 0) {
            throw new IllegalArgumentException("minSmallHeapSize can't be less than 0");
        }

        this.minSmallHeapSize = minSmallHeapSize;
        return this;
    }
}
