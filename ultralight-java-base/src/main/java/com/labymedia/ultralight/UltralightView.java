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

package com.labymedia.ultralight;

import com.labymedia.ultralight.annotation.NativeCall;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.annotation.Unsigned;
import com.labymedia.ultralight.ffi.ObjectWithHandle;
import com.labymedia.ultralight.ffi.RefPtr;
import com.labymedia.ultralight.input.UltralightKeyEvent;
import com.labymedia.ultralight.input.UltralightKeyEventType;
import com.labymedia.ultralight.input.UltralightMouseEvent;
import com.labymedia.ultralight.input.UltralightScrollEvent;
import com.labymedia.ultralight.javascript.JavascriptContextLock;
import com.labymedia.ultralight.javascript.JavascriptEvaluationException;
import com.labymedia.ultralight.plugin.loading.UltralightLoadListener;
import com.labymedia.ultralight.plugin.render.UltralightRenderTarget;
import com.labymedia.ultralight.plugin.view.UltralightViewListener;

import java.util.Objects;

/**
 * A View is similar to a tab in a browser-- you load web content into it and display it however you want. @see
 * Renderer::CreateView
 * <p>
 * <b>The API is currently not thread-safe, all calls must be made on the
 * main thread.</b>
 */
@NativeType("ultralight::RefPtr<ultralight::View>")
public class UltralightView implements ObjectWithHandle {
    // C++ RefPtr of the object
    private final RefPtr<UltralightView> ptr;

    /**
     * Constructs a new {@link UltralightView} from the given native {@link RefPtr}.
     *
     * @param ptr The pointer managing the view instance
     */
    @NativeCall
    private UltralightView(RefPtr<UltralightView> ptr) {
        this.ptr = ptr;
    }

    /**
     * Get the URL of the current page loaded into this View, if any.
     *
     * @return The url of the current page, or an empty String if none
     */
    public native String url();

    /**
     * Get the title of the current page loaded into this View, if any.
     *
     * @return The title of the current page, or an empty String if none
     */
    public native String title();

    /**
     * Get the width of the View, in pixels.
     *
     * @return The width of the view
     */
    public native @NativeType("uint32_t")
    @Unsigned
    long width();

    /**
     * Get the height of the View, in pixels.
     *
     * @return The height of the view
     */
    public native @NativeType("uint32_t")
    @Unsigned
    long height();

    /**
     * Check if the main frame of the page is currently loading.
     *
     * @return Whether the main frame is currently loading
     */
    public native boolean isLoading();

    /**
     * Get the offscreen {@link UltralightRenderTarget} for the View.
     * <p>
     * Only valid when the {@link com.labymedia.ultralight.config.UltralightViewConfig#isAccelerated(boolean)} is {@code
     * true}.
     * <p>
     * You can use this with your GPUDriver implementation to bind and display the corresponding texture in your
     * application.
     *
     * @return The offscreen render target
     */
    public native UltralightRenderTarget renderTarget();

    /**
     * Get the Surface for the View (native pixel buffer container).
     * <b>Only valid when the GPU renderer is disabled in Config.</b>
     * <p>
     * TODO: Fix doc/implement
     *
     * @return The surface of this view, or {@code null}, if GPU rendering is enabled
     */
    // Not implemented yet:
    // The default Surface is BitmapSurface but you can provide your
    // own Surface implementation via Platform::set_surface_factory.
    public native UltralightSurface surface();

    /**
     * Load a raw string of HTML, the View will navigate to it as a new page.
     *
     * @param html The raw HTML string to load.
     */
    public void loadHTML(String html) {
        loadHTML(html, null, false);
    }

    /**
     * @param html The raw HTML string to load.
     * @param url  An optional URL for this load (to make it appear as if we we loaded this HTML from a certain URL).
     *             Can be used for resolving relative URLs and cross-origin rules.
     */
    public void loadHTML(String html, String url) {
        loadHTML(html, url, false);
    }

    /**
     * Load a raw string of HTML, the View will navigate to it as a new page.
     *
     * @param html         The raw HTML string to load.
     * @param url          An optional URL for this load (to make it appear as if we we loaded this HTML from a certain
     *                     URL). Can be used for resolving relative URLs and cross-origin rules.
     * @param addToHistory Whether or not this load should be added to the session's history (back/forward list).
     */
    public native void loadHTML(String html, String url, boolean addToHistory);

    /**
     * Load a URL, the View will navigate to it as a new page.
     * <p>
     * you can use File URLs (eg, file:///page.html) but you must define your own FileSystem implementation if you are
     * not using AppCore.
     *
     * @param url The url to load
     * @see UltralightPlatform#usePlatformFileSystem(String)
     */
    public native void loadURL(String url);

    /**
     * Resize View to a certain size.
     *
     * @param width  The initial width, in pixels.
     * @param height The initial height, in pixels.
     */
    public native void resize(@NativeType("uint32_t") @Unsigned long width, @NativeType(
            "uint32_t") @Unsigned long height);

    /**
     * Acquire the page's JSContext for use with the JavaScriptCore API.
     * <p>
     * While the lock is being hold no script will be executed by the web engine.
     * <p>
     * The context gets reset every time the view navigates, to populate it with custom values the {@link
     * UltralightLoadListener#onWindowObjectReady(long, boolean, String)} and {@link
     * UltralightLoadListener#onDOMReady(long, boolean, String)} methods are recommended.
     *
     * @return The script context of the view
     */
    public native JavascriptContextLock lockJavascriptContext();

    /**
     * Helper function to evaluate a raw string of JavaScript and return the result as a String.
     * <p>
     * You do not need to lock the JS context, it is done automatically.
     *
     * @param script A string of JavaScript to evaluate in the main frame.
     * @return The JavaScript result typecast to a String.
     * @throws JavascriptEvaluationException If the evaluated script throws an exception
     */
    public native String evaluateScript(String script) throws JavascriptEvaluationException;

    /**
     * Whether or not we can navigate backwards in history.
     *
     * @return Whether the view is able to navigate backwards
     */
    public native boolean canGoBack();

    /**
     * Whether or not we can navigate forwards in history.
     *
     * @return Whether the view is able to to navigate forwards
     */
    public native boolean canGoForward();

    /**
     * Navigate backwards in history.
     */
    public native void goBack();

    /**
     * Navigate forwards in history.
     */
    public native void goForward();

    /**
     * Navigate to an arbitrary offset in history.
     *
     * @param offset The offset to navigate to in the history
     */
    public native void goToHistoryOffset(int offset);

    /**
     * Reload current page.
     */
    public native void reload();

    /**
     * Stop all page loads.
     */
    public native void stop();

    /**
     * Give focus to the View.
     * <p>
     * You should call this to give visual indication that the View has input focus (changes active text selection
     * colors, for example).
     */
    public native void focus();

    /**
     * Remove focus from the View and unfocus any focused input elements.
     * <p>
     * You should call this to give visual indication that the View has lost input focus.
     */
    public native void unfocus();

    /**
     * Whether or not the View has focus.
     *
     * @return Whether the view has focus
     */
    public native boolean hasFocus();

    /**
     * Whether or not the View has an input element with visible keyboard focus (indicated by a blinking caret).
     * <p>
     * You can use this to decide whether or not the View should consume keyboard input events (useful in games with
     * mixed UI and key handling).
     *
     * @return Whether the view has input focus
     */
    public native boolean hasInputFocus();

    /**
     * Fire a keyboard event. Only {@link UltralightKeyEventType#CHAR CHAR} events actually generate text in input
     * fields.
     *
     * @param event The event to fire
     */
    public native void fireKeyEvent(UltralightKeyEvent event);

    /**
     * Fire a mouse event.
     *
     * @param event The event to fire
     */
    public native void fireMouseEvent(UltralightMouseEvent event);

    /**
     * Fire a scroll event
     *
     * @param event The event to fire
     */
    public native void fireScrollEvent(UltralightScrollEvent event);

    /**
     * Set a {@link UltralightViewListener} to receive callbacks for View-related events.
     *
     * @param listener The listener to set
     */
    public native void setViewListener(UltralightViewListener listener);

    /**
     * Set a {@link UltralightLoadListener} to receive callbacks for Load-related events.
     *
     * @param listener The listener to set
     */
    public native void setLoadListener(UltralightLoadListener listener);

    /**
     * Set whether or not this View should be repainted during the next call to {@link UltralightRenderer#render()}.
     * <p>
     * This flag is automatically set whenever the page content changes but you can set it directly in case you need to
     * force a repaint.
     *
     * @param needsPaint Whether the needs a re paint
     */
    public native void setNeedsPaint(boolean needsPaint);

    /**
     * Whether or not this View should be repainted during the next call to {@link UltralightRenderer#render()}.
     *
     * @return Whether the view needs a re paint during the next render
     */
    public native boolean needsPaint();

    /**
     * Sets the viewport scale factor.
     *
     * @param deviceScale the scale factor to set
     */
    public native void setDeviceScale(double deviceScale);

    public native double getDeviceScale();

    /**
     * Get the inspector for this View, this is useful for debugging and inspecting pages locally. This will only
     * succeed if you have the inspector assets in your filesystem-- the inspector will look for
     * file:///inspector/Main.html when it first loads.
     * <p>
     * The inspector View is owned by the View and lazily-created on first call. The initial dimensions are 10x10, you
     * should call {@link #resize(long, long)} on the returned View to resize it to your desired dimensions.
     *
     * @return An inspector view for the current view
     */
    public native UltralightView inspector();

    @Override
    public long getHandle() {
        return ptr.getHandle();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (!(o instanceof UltralightView)) {
            return false;
        }
        UltralightView that = (UltralightView) o;
        return ptr.getHandle() == that.ptr.getHandle();
    }

    @Override
    public int hashCode() {
        return Objects.hash(ptr.getHandle());
    }
}
