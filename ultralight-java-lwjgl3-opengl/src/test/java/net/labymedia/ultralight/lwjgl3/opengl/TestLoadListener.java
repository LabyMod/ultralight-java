/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package net.labymedia.ultralight.lwjgl3.opengl;

import net.labymedia.ultralight.Databind;
import net.labymedia.ultralight.DatabindConfiguration;
import net.labymedia.ultralight.DatabindJavascriptClass;
import net.labymedia.ultralight.UltralightView;
import net.labymedia.ultralight.api.JavaAPI;
import net.labymedia.ultralight.javascript.JavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptContextLock;
import net.labymedia.ultralight.javascript.JavascriptGlobalContext;
import net.labymedia.ultralight.javascript.JavascriptObject;
import net.labymedia.ultralight.plugin.loading.UltralightLoadListener;

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
        try (JavascriptContextLock lock = view.lockJavascriptContext()) {
            JavascriptContext context = lock.getContext();
            JavascriptGlobalContext globalContext = context.getGlobalContext();

            JavascriptObject globalObject = globalContext.getGlobalObject();

            Databind databind = new Databind(DatabindConfiguration.builder()
                    .contextProviderFactory(new TestContextProvider.Factory(view)).build());
            JavascriptObject javaApi = context.makeObject(databind.toJavascript(JavaAPI.class),
                    new DatabindJavascriptClass.Data(new JavaAPI(databind), null));
            globalObject.setProperty("java", javaApi, 0);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @Override
    public void onDOMReady(long frameId, boolean isMainFrame, String url) {

    }
}
