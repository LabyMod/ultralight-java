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

import net.labymedia.ultralight.UltralightView;
import net.labymedia.ultralight.context.ContextProvider;
import net.labymedia.ultralight.context.ContextProviderFactory;
import net.labymedia.ultralight.javascript.JavascriptContextLock;
import net.labymedia.ultralight.javascript.JavascriptValue;

public class TestContextProvider implements ContextProvider {
    private final UltralightView view;

    private TestContextProvider(UltralightView view) {
        this.view = view;
    }

    @Override
    public JavascriptContextLock getContext() {
        return view.lockJavascriptContext();
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
