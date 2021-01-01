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

package com.labymedia.ultralight.plugin.loading;

import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.annotation.Unsigned;

/**
 * Interface for Load-related events.
 * <p>
 * For more info @see View::set_load_listener
 */
@NativeType("ultralight::LoadListener")
public interface UltralightLoadListener {
    /**
     * Called when the page begins loading a new URL into the main frame.
     *
     * @param frameId     The id of the frame that has begun loading
     * @param isMainFrame Whether the frame is the main frame
     * @param url         The url that the frame started to load
     */
    void onBeginLoading(@NativeType("uint64_t") @Unsigned long frameId, boolean isMainFrame, String url);

    /**
     * Called when the page finishes loading a URL into the main frame.
     *
     * @param frameId     The id of the frame that finished loading
     * @param isMainFrame Whether the frame is the main frame
     * @param url         The url the frame has loaded
     */
    void onFinishLoading(@NativeType("uint64_t") @Unsigned long frameId, boolean isMainFrame, String url);

    /**
     * Called when an error occurs while loading a URL into the main frame.
     *
     * @param frameId     The id of the frame that failed to load
     * @param isMainFrame Whether the frame is the main frame
     * @param url         The url that failed to load
     * @param description A description of the error
     * @param errorDomain The domain that failed to load
     * @param errorCode   An error code indicating the error reason
     */
    void onFailLoading(
            @NativeType("uint64_t") @Unsigned long frameId,
            boolean isMainFrame,
            String url,
            String description,
            String errorDomain,
            int errorCode
    );

    /**
     * Called when the session history (back/forward state) is modified.
     */
    void onUpdateHistory();

    /**
     * Called when the JavaScript window object is reset for a new page load. This is called before any scripts are
     * executed on the page and is the best time to setup any initial JavaScript state or bindings.
     * <p>
     * The document is not guaranteed to be loaded/parsed at this point. If you need to make any JavaScript calls that
     * are dependent on DOM elements or scripts on the page, use OnDOMReady instead.
     *
     * @param frameId     The id of the frame that the object became ready in
     * @param isMainFrame Whether the frame is the main frame
     * @param url         The url that the frame currently contains
     */
    void onWindowObjectReady(@NativeType("uint64_t") @Unsigned long frameId, boolean isMainFrame, String url);

    /**
     * Called when all JavaScript has been parsed and the document is ready.
     * <p>
     * This is the best time to make any JavaScript calls that are dependent on DOM elements or scripts on the page.
     *
     * @param frameId     The id of the frame that the DOM became ready in
     * @param isMainFrame Whether the frame is the main frame
     * @param url         The url that the frame currently contains
     */
    void onDOMReady(@NativeType("uint64_t") @Unsigned long frameId, boolean isMainFrame, String url);
}
