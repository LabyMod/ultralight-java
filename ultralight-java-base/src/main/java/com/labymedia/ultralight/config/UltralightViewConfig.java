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

@NativeType("ultralight::ViewConfig")
public class UltralightViewConfig {
    /**
     * TODO: Adjust source links
     * <p>
     * When enabled, the View will be rendered to an offscreen GPU texture
     * using the GPU driver set in Platform::set_gpu_driver. You can fetch
     * details for the texture via View::render_target.
     * <p>
     * When disabled (the default), the View will be rendered to an offscreen
     * pixel buffer. This pixel buffer can optionally be provided by the user --
     * for more info see <Ultralight/platform/Surface.h> and View::surface.
     */
    @Native
    private boolean isAccelerated = false;

    @Native
    private boolean isTransparent = false;

    @Native
    private double initialDeviceScale = 1.0;

    @Native
    private boolean initialFocus = true;

    /**
     * Whether or not images should be enabled.
     */
    @Native
    private boolean enableImages = true;

    /**
     * Whether or not JavaScript should be enabled.
     */
    @Native
    private boolean enableJavascript = true;

    /**
     * Default font-family to use.
     */
    @Native
    @NativeType("ultralight::String16")
    private String fontFamilyStandard = "Times New Roman";

    /**
     * Default font-family to use for fixed fonts. (pre/code)
     */
    @Native
    @NativeType("ultralight::String16")
    private String fontFamilyFixed = "Courier New";

    /**
     * Default font-family to use for serif fonts.
     */
    @Native
    @NativeType("ultralight::String16")
    private String fontFamilySerif = "Times New Roman";

    /**
     * Default font-family to use for sans-serif fonts.
     */
    @Native
    @NativeType("ultralight::String16")
    private String fontFamilySansSerif = "Arial";

    /**
     * Default user-agent string.
     */
    @Native
    @NativeType("ultralight::String16")
    private String userAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) " +
            "AppleWebKit/608.3.10 (KHTML, like Gecko) " +
            "Ultralight/1.3.0 UltralightJava/0.4.0 Safari/608.3.10";

    /**
     * Sets the isAccelerated field of this instance.
     *
     * @param isAccelerated The new value of the field
     * @return this
     * @see #isAccelerated
     */
    public UltralightViewConfig isAccelerated(boolean isAccelerated) {
        this.isAccelerated = isAccelerated;
        return this;
    }

    /**
     * Sets the isTransparent field of this instance.
     *
     * @param isTransparent The new value of the field
     * @return this
     * @see #isTransparent
     */
    public UltralightViewConfig isTransparent(boolean isTransparent) {
        this.isTransparent = isTransparent;
        return this;
    }

    /**
     * Sets the initialDeviceScale field of this instance.
     *
     * @param initialDeviceScale The new value of the field
     * @return this
     * @see #initialDeviceScale
     */
    public UltralightViewConfig initialDeviceScale(double initialDeviceScale) {
        this.initialDeviceScale = initialDeviceScale;
        return this;
    }

    /**
     * Sets the initialFocus field of this instance.
     *
     * @param initialFocus The new value of the field
     * @return this
     * @see #initialFocus
     */
    public UltralightViewConfig initialFocus(boolean initialFocus) {
        this.initialFocus = initialFocus;
        return this;
    }

    /**
     * Sets the enableImages field of this instance.
     *
     * @param enableImages The new value of the field
     * @return this
     * @see #enableImages
     */
    public UltralightViewConfig enableImages(boolean enableImages) {
        this.enableImages = enableImages;
        return this;
    }

    /**
     * Sets the enableJavascript field of this instance.
     *
     * @param enableJavascript The new value of the field
     * @return this
     * @see #enableJavascript
     */
    public UltralightViewConfig enableJavascript(boolean enableJavascript) {
        this.enableJavascript = enableJavascript;
        return this;
    }

    /**
     * Sets the fontFamilyStandard field of this instance.
     *
     * @param fontFamilyStandard The new value of the field
     * @return this
     * @see #fontFamilyStandard
     */
    public UltralightViewConfig fontFamilyStandard(String fontFamilyStandard) {
        this.fontFamilyStandard = fontFamilyStandard;
        return this;
    }

    /**
     * Sets the fontFamilyFixed field of this instance.
     *
     * @param fontFamilyFixed The new value of the field
     * @return this
     * @see #fontFamilyFixed
     */
    public UltralightViewConfig fontFamilyFixed(String fontFamilyFixed) {
        this.fontFamilyFixed = fontFamilyFixed;
        return this;
    }

    /**
     * Sets the fontFamilySerif field of this instance.
     *
     * @param fontFamilySerif The new value of the field
     * @return this
     * @see #fontFamilySerif
     */
    public UltralightViewConfig fontFamilySerif(String fontFamilySerif) {
        this.fontFamilySerif = fontFamilySerif;
        return this;
    }

    /**
     * Sets the fontFamilySansSerif field of this instance.
     *
     * @param fontFamilySansSerif The new value of the field
     * @return this
     * @see #fontFamilySansSerif
     */
    public UltralightViewConfig fontFamilySansSerif(String fontFamilySansSerif) {
        this.fontFamilySansSerif = fontFamilySansSerif;
        return this;
    }

    /**
     * Sets the userAgent field of this instance.
     *
     * @param userAgent The new value of the field
     * @return this
     * @see #userAgent
     */
    public UltralightViewConfig userAgent(String userAgent) {
        this.userAgent = userAgent;
        return this;
    }
}
