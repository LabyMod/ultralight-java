package net.janrupf.ultralight.plugin.view;

import net.janrupf.ultralight.UltralightView;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.annotation.Unsigned;
import net.janrupf.ultralight.input.UltralightCursor;
import net.janrupf.ultralight.math.IntRect;

/**
 * Interface for View-related events
 */
public interface UltralightViewListener {
    /**
     * Called when the page title changes.
     *
     * @param title The new page title
     */
    void onChangeTitle(String title);

    /**
     * Called when the page URL changes.
     *
     * @param url The new page url
     */
    void onChangeURL(String url);

    /**
     * Called when the tooltip changes (usually as result of a mouse hover).
     *
     * @param tooltip The new page tooltip
     */
    void onChangeTooltip(String tooltip);

    /**
     * Called when the mouse cursor changes.
     *
     * @param cursor The new page cursor
     */
    void onChangeCursor(UltralightCursor cursor);

    /**
     * Called when a message is added to the console (useful for errors / debug).
     *
     * @param source       The source the message originated from
     * @param level        The severity of the message
     * @param message      The message itself
     * @param lineNumber   The line the message originated from
     * @param columnNumber The column the message originated from
     * @param sourceId     The id of the source
     */
    void onAddConsoleMessage(
            MessageSource source,
            MessageLevel level,
            String message,
            @NativeType("uint23_t") @Unsigned long lineNumber,
            @NativeType("uint32_t") @Unsigned long columnNumber,
            String sourceId
    );

    /**
     * Called when the page wants to create a new View.
     * <p>
     * This is usually the result of a user clicking a link with target="_blank" or by JavaScript calling
     * window.open(url).
     * <p>
     * To allow creation of these new Views, you should create a new View in this callback (eg, {@link
     * net.janrupf.ultralight.UltralightRenderer#createView(long, long, boolean)}), resize it to your container, and
     * return it. You are responsible for displaying the returned View.
     *
     * @param openerUrl The URL of the page that initiated this request
     * @param targetUrl The URL that the new View will navigate to
     * @param isPopup   Whether or not this was triggered by window.open()
     * @param popupRect Popups can optionally request certain dimensions and coordinates via window.open(). You can
     *                  choose to respect these or not by resizing/moving the View to this rect.
     * @return Returns a {@link UltralightView} to use to satisfy the the request (or return {@code null} if you want to
     *         block the action).
     */
    UltralightView onCreateChildView(String openerUrl, String targetUrl, boolean isPopup, IntRect popupRect);
}
