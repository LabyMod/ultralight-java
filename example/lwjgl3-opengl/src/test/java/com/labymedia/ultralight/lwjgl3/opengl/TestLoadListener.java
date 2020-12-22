/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
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

package com.labymedia.ultralight.lwjgl3.opengl;

import com.labymedia.ultralight.Databind;
import com.labymedia.ultralight.DatabindConfiguration;
import com.labymedia.ultralight.DatabindJavascriptClass;
import com.labymedia.ultralight.UltralightView;
import com.labymedia.ultralight.api.JavaAPI;
import com.labymedia.ultralight.javascript.JavascriptContext;
import com.labymedia.ultralight.javascript.JavascriptContextLock;
import com.labymedia.ultralight.javascript.JavascriptGlobalContext;
import com.labymedia.ultralight.javascript.JavascriptObject;
import com.labymedia.ultralight.plugin.loading.UltralightLoadListener;

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
