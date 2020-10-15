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
