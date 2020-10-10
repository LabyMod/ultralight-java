package net.labymedia.ultralight.call;

import net.labymedia.ultralight.javascript.JavascriptValue;

import java.lang.reflect.Executable;
import java.util.Collection;

/**
 * Describes an abstraction layer around the system determining which method to invoke on an incoming
 * call from Javascript.
 */
public interface MethodChooser {
    /**
     * Chooses the best matching method based on the arguments incoming from Javascript.
     *
     * @param possibilities    All executables which could possibly be targeted by this call
     * @param javascriptValues The arguments being passed from Javascript
     * @param <T>              The target executable type
     * @return Information on how to call the chosen executable
     * @throws IllegalStateException If the target executable can not be determined
     */
    <T extends Executable> CallData<T> choose(Collection<? extends T> possibilities, JavascriptValue[] javascriptValues);

    /**
     * Chooses the best matching method based on the arguments incoming from Javascript.
     *
     * @param possibilities        All executables which could possibly be targeted by this call
     * @param sourceParameterTypes The desired types for the parameters
     * @param javascriptValues     The arguments being passed from Javascript
     * @param <T>                  The target executable type
     * @return Information on how to call the chosen executable
     * @throws IllegalStateException If the target executable can not be determined
     */
    <T extends Executable> CallData<T> choose(
            Collection<? extends T> possibilities, Class<?>[] sourceParameterTypes, JavascriptValue[] javascriptValues);
}
