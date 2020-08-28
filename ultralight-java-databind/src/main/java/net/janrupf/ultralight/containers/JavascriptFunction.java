package net.janrupf.ultralight.containers;

import net.janrupf.ultralight.javascript.JavascriptClass;

import java.lang.invoke.MethodHandle;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

public final class JavascriptFunction {
    private final JavascriptClass functionClass;

    private final Map<String, MethodHandle> methodHandles = new HashMap<>();

    public JavascriptFunction(final JavascriptClass functionClass) {
        this.functionClass = functionClass;
    }

    public JavascriptClass functionClass() {
        return functionClass;
    }

    public void addMethodHandle(final MethodHandle method) {
        for (Class<?> type : method.type().parameterList()) {
            if (type.isPrimitive()) {

            }
        }
    }
}
