package net.labymedia.ultralight.utils;

import net.labymedia.ultralight.javascript.JavascriptValue;

import java.lang.reflect.Executable;
import java.util.Collection;

public interface MethodChooser {
    Executable choose(Collection<? extends Executable> possibilities, JavascriptValue... javascriptValues);
    Executable choose(Collection<? extends Executable> possibilities, Class<?>... sourceParameterTypes);
}
