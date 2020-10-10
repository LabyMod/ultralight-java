package net.labymedia.ultralight.call;

import java.lang.reflect.Executable;

public class CallData<T extends Executable> {
    private final T target;
    private final VarArgsType varArgsType;

    public CallData(T target, VarArgsType varArgsType) {
        this.target = target;
        this.varArgsType = varArgsType;
    }

    public T getTarget() {
        return target;
    }

    public VarArgsType getVarArgsType() {
        return varArgsType;
    }

    public enum VarArgsType {
        NONE,
        COMPACT,
        PASS_THROUGH,
        EMPTY
    }
}
