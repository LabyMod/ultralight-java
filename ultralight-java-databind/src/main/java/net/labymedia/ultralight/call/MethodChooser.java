package net.labymedia.ultralight.call;

import net.labymedia.ultralight.javascript.JavascriptValue;

import java.lang.reflect.Executable;
import java.util.Collection;

public interface MethodChooser {
    <T extends Executable> CallData<T> choose(Collection<? extends T> possibilities, JavascriptValue[] javascriptValues);
    <T extends Executable> CallData<T> choose(Collection<? extends T> possibilities, Class<?>[] sourceParameterTypes, JavascriptValue[] javascriptValues);
}
