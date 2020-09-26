package net.labymedia.ultralight.utils;

import java.lang.reflect.Array;
import java.lang.reflect.Executable;
import java.util.List;

public final class VarArgsUtils {
    private VarArgsUtils() { }

    public static List<Object> toVarArgs(List<Object> parameters, Executable executable) {
        int size = parameters.size() - executable.getParameterCount() + 1;
        int methodParameterCount = executable.getParameterCount();

        if (size < 0) size = 0;

        Object array = Array.newInstance(executable.getParameterTypes()[methodParameterCount - 1].getComponentType(), size);

        if (size > 0) {
            for (int i = methodParameterCount - 1; i < parameters.size(); i++) {
                Array.set(array, i - methodParameterCount + 1, parameters.get(i));
            }
        }

        parameters = parameters.subList(0, methodParameterCount - 1);
        parameters.add(array);

        return parameters;
    }
}
