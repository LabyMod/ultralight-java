package net.janrupf.ultralight.lwjgl3.opengl;

import net.janrupf.ultralight.UltralightView;
import net.janrupf.ultralight.javascript.JavascriptContext;
import net.janrupf.ultralight.javascript.JavascriptContextLock;
import net.janrupf.ultralight.javascript.JavascriptGlobalContext;
import net.janrupf.ultralight.javascript.JavascriptValue;
import net.janrupf.ultralight.plugin.loading.UltralightLoadListener;

/**
 * Test load listener for the main view.
 */
public class TestLoadListener implements UltralightLoadListener {
    private final UltralightView view;

    public TestLoadListener(UltralightView view) {
        this.view = view;
    }

    private String frameName(long frameId, boolean isMainFrame, String url) {
        return "[" + (isMainFrame ? "MainFrame" : "Frame") + " " + frameId + " (" + url + ")]: ";
    }

    @Override
    public void onBeginLoading(long frameId, boolean isMainFrame, String url) {
        System.out.println(frameName(frameId, isMainFrame, url) + "The view is about to load");
    }

    @Override
    public void onFinishLoading(long frameId, boolean isMainFrame, String url) {
        System.out.println(frameName(frameId, isMainFrame, url) + "The view finished loading");
    }

    @Override
    public void onFailLoading(
            long frameId, boolean isMainFrame, String url, String description, String errorDomain, int errorCode) {
        System.err.println(frameName(frameId, isMainFrame, url) +
                "Failed to load " + errorDomain + ", " + errorCode + "(" + description + ")");
    }

    @Override
    public void onUpdateHistory() {
        System.out.println("The view has updated the history");
    }

    @Override
    public void onWindowObjectReady(long frameId, boolean isMainFrame, String url) {
        JavascriptValue value;

        try(JavascriptContextLock lock = view.lockJavascriptContext()) {
            JavascriptContext context = lock.getContext();
            JavascriptGlobalContext globalContext = context.getGlobalContext();

            System.out.println("New context name: " + globalContext.getName());
        }
    }

    @Override
    public void onDOMReady(long frameId, boolean isMainFrame, String url) {
        System.out.println(frameName(frameId, isMainFrame, url) + "The DOM is ready");
    }
}
