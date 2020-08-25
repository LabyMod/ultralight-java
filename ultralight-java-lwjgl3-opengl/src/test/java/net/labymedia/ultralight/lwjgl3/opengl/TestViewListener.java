package net.labymedia.ultralight.lwjgl3.opengl;

import net.labymedia.ultralight.UltralightView;
import net.labymedia.ultralight.input.UltralightCursor;
import net.labymedia.ultralight.math.IntRect;
import net.labymedia.ultralight.plugin.view.MessageLevel;
import net.labymedia.ultralight.plugin.view.MessageSource;
import net.labymedia.ultralight.plugin.view.UltralightViewListener;

/**
 * Test view listener for the main view
 */
public class TestViewListener implements UltralightViewListener {
    private final TestCursorManager cursorManager;

    public TestViewListener(TestCursorManager cursorManager) {
        this.cursorManager = cursorManager;
    }

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
        this.cursorManager.notifyCursorUpdated(cursor);
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
