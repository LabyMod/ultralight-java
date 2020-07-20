package net.janrupf.ultralight.lwjgl3.opengl;

import net.janrupf.ultralight.UltralightView;
import net.janrupf.ultralight.input.UltralightCursor;
import net.janrupf.ultralight.math.IntRect;
import net.janrupf.ultralight.plugin.view.MessageLevel;
import net.janrupf.ultralight.plugin.view.MessageSource;
import net.janrupf.ultralight.plugin.view.UltralightViewListener;

/**
 * Test view listener for the main view
 */
public class TestViewListener implements UltralightViewListener {
    @Override
    public void onChangeTitle(String title) {
        System.out.println("View title has changed: " + title);
    }

    @Override
    public void onChangeURL(String url) {
        System.out.println("View url has changed: " + url);
    }

    @Override
    public void onChangeTooltip(String tooltip) {
        // System.out.println("View tooltip has changed: " + tooltip);
    }

    @Override
    public void onChangeCursor(UltralightCursor cursor) {
        // System.out.println("View cursor has changed: " + cursor.name());
    }

    @Override
    public void onAddConsoleMessage(
            MessageSource source,
            MessageLevel level,
            String message,
            long lineNumber,
            long columnNumber,
            String sourceId
    ) {
        System.out.println(
                "View message: ["
                        + source.name() + "/" + level.name() + "] "
                        + sourceId + ":" + lineNumber + ":" + columnNumber + ": " + message);
    }

    @Override
    public UltralightView onCreateChildView(String openerUrl, String targetUrl, boolean isPopup, IntRect popupRect) {
        System.out.println("View wants child: ");
        System.out.println("\tFrom: " + openerUrl);
        System.out.println("\tTo: " + targetUrl);
        System.out.println("\tIs popup: " + isPopup);
        if(popupRect.isValid()) {
            System.out.println("\tTarget rect: (" +
                    popupRect.x() + "," + popupRect.y() + " -> " + popupRect.width() + "," + popupRect.height() + ")");
        } else {
            System.out.println("\tTarget rect: NONE");
        }
        System.out.println("Cancelling request, multi view not implemented");
        return null;
    }
}
