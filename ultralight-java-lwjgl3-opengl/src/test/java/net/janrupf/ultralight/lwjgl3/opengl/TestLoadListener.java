package net.janrupf.ultralight.lwjgl3.opengl;

import net.janrupf.ultralight.plugin.loading.UltralightLoadListener;

/**
 * Test load listener for the main view.
 */
public class TestLoadListener implements UltralightLoadListener {
    @Override
    public void onBeginLoading() {
        System.out.println("The view is about to load");
    }

    @Override
    public void onFinishLoading() {
        System.out.println("The view finished loading");
    }

    @Override
    public void onFailLoading(String url, String description, String errorDomain, int errorCode) {
        System.err.println("The view failed to load " + url + " on " + errorDomain + ": " + description + " (" +
                errorCode + ")");
    }

    @Override
    public void onUpdateHistory() {
        System.out.println("The view has updated the history");
    }

    @Override
    public void onWindowObjectReady() {
        System.out.println("The window object is ready");
    }

    @Override
    public void onDOMReady() {
        System.out.println("The DOM is ready");
    }
}
