package net.labymedia.ultralight.utils;

import java.lang.reflect.Executable;
import java.util.Collection;

public interface MethodChooser {
    Executable choose(Collection<? extends Executable> possibilities, Class<?>... sourceParameterTypes);
}
