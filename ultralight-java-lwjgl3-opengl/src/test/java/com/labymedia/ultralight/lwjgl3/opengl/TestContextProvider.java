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

import com.labymedia.ultralight.UltralightView;
import com.labymedia.ultralight.context.ContextProviderFactory;
import com.labymedia.ultralight.context.ContextProvider;
import com.labymedia.ultralight.javascript.JavascriptContext;
import com.labymedia.ultralight.javascript.JavascriptContextLock;
import com.labymedia.ultralight.javascript.JavascriptValue;

import java.util.function.Consumer;

public class TestContextProvider implements ContextProvider {
    private final UltralightView view;

    private TestContextProvider(UltralightView view) {
        this.view = view;
    }

    @Override
    public void syncWithJavascript(Consumer<JavascriptContextLock> callback) {
        try(JavascriptContextLock lock = view.lockJavascriptContext()) {
            callback.accept(lock);
        }
    }

    public static class Factory implements ContextProviderFactory {
        private final UltralightView view;

        public Factory(UltralightView view) {
            this.view = view;
        }

        @Override
        public ContextProvider bindProvider(JavascriptValue value) {
            // We only have one view, ignore the value
            return new TestContextProvider(view);
        }
    }
}