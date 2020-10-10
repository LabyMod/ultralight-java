package net.labymedia.ultralight.call;

import net.labymedia.ultralight.api.InjectJavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptObject;
import net.labymedia.ultralight.javascript.JavascriptValue;
import net.labymedia.ultralight.utils.JavascriptConversionUtils;

import java.lang.reflect.Executable;
import java.lang.reflect.Parameter;
import java.util.*;

public final class HeuristicMethodChooser implements MethodChooser {
    @Override
    public <T extends Executable> CallData<T> choose(Collection<? extends T> possibilities, JavascriptValue... javascriptValues) {
        Class<?>[] parameterTypes = new Class<?>[javascriptValues.length];

        for (int i = 0; i < parameterTypes.length; i++) {
            parameterTypes[i] = JavascriptConversionUtils.determineType(javascriptValues[i]);
        }

        return choose(possibilities, parameterTypes, javascriptValues);
    }

    @Override
    public <T extends Executable> CallData<T> choose(Collection<? extends T> possibilities, Class<?>[] sourceParameterTypes, JavascriptValue[] javascriptValues) {
        int penalty = Integer.MAX_VALUE;
        Set<CallData<T>> availableMethods = new HashSet<>();

        tryNextMethod:
        for (T executable : possibilities) {
            boolean injectContext = executable.isAnnotationPresent(InjectJavascriptContext.class);

            int currentPenalty = 0;
            CallData.VarArgsType varArgsType = null;

            Parameter[] parameters = executable.getParameters();
            if (parameters.length != sourceParameterTypes.length + (injectContext ? 1 : 0)) {
                if (!executable.isVarArgs() || sourceParameterTypes.length < parameters.length - (injectContext ? 0 : 1)) {
                    // Parameter count is not enough, even if the variable arguments are not filled at all
                    continue;
                }
            }

            for (int i = 0; i < parameters.length - (injectContext ? 1 : 0); i++) {
                if (i + (injectContext ? 1 : 0) == parameters.length - 1 && executable.isVarArgs()) {
                    // Last parameter is var args, special handling required
                    if (sourceParameterTypes.length < parameters.length) {
                        // Var args not supplied at all
                        varArgsType = CallData.VarArgsType.NONE;
                    } else if (sourceParameterTypes[i].isArray() && sourceParameterTypes.length == parameters.length) {
                        if (parameters[i + (injectContext ? 1 : 0)].getType().isAssignableFrom(sourceParameterTypes[i])) {
                            // Var args array can be passed through as the array types match
                            varArgsType = CallData.VarArgsType.PASS_THROUGH;
                        } else {
                            if (!parameters[i + (injectContext ? 1 : 0)].getType().getComponentType().isAssignableFrom(sourceParameterTypes[i])) {
                                // Method parameter can't be compacted down
                                continue tryNextMethod;
                            }
                            // Compacting the last argument is enough to make it compatible
                            varArgsType = CallData.VarArgsType.COMPACT;
                        }
                    } else {
                        // The arguments need to be compacted
                        varArgsType = CallData.VarArgsType.COMPACT;

                        for (int x = i; x < sourceParameterTypes.length; x++) {
                            int argPenalty = calculatePenalty(
                                    parameters[i + (injectContext ? 1 : 0)].getType().getComponentType(),
                                    sourceParameterTypes[x],
                                    javascriptValues[x]
                            );
                            if (argPenalty < 0) {
                                // Argument is incompatible, search next method
                                continue tryNextMethod;
                            }

                            currentPenalty += argPenalty;
                        }
                    }
                } else {
                    int argPenalty = calculatePenalty(
                            parameters[i + (injectContext ? 1 : 0)].getType(),
                            sourceParameterTypes[i],
                            javascriptValues[i]
                    );
                    if (argPenalty < 0) {
                        // Argument is incompatible, search next method
                        continue tryNextMethod;
                    }

                    currentPenalty += argPenalty;
                }
            }

            if (currentPenalty < penalty) {
                availableMethods.clear();
                availableMethods.add(new CallData<>(executable, varArgsType));
            } else if (currentPenalty == penalty) {
                availableMethods.add(new CallData<>(executable, varArgsType));
            }
        }

        if (availableMethods.size() > 1) {
            throw new IllegalStateException("Ambiguous argument types, could not determine methods");
        } else if (availableMethods.isEmpty()) {
            throw new IllegalStateException("No matching method found");
        }

        return availableMethods.iterator().next();
    }

    // D extends C
    // C extends B
    // B extends A
    //
    // something(A) => ...
    // something(B) => ...
    // something(D) => ...
    //
    // something(new D()) => something(A): penalty = 3, something(B): penalty = 2, something(D): penalty = 0

    private int calculatePenalty(Class<?> target, Class<?> source, JavascriptValue value) {
        if (target == JavascriptValue.class) {
            return 0;
        } else if (target == JavascriptObject.class) {
            return value.isObject() ? 0 : -1;
        } else if (!target.isAssignableFrom(source)) {
            return -1;
        } else if (target == source) {
            return 0;
        }

        class ClassWithPrio {
            Class clazz;
            int prio;

            public ClassWithPrio(Class clazz, int prio) {
                this.clazz = clazz;
                this.prio = prio;
            }
        }

        int penalty = 0;
        HashMap<Class, Integer> dist = new HashMap<>();
        Queue<ClassWithPrio> queue = new LinkedList<>();
        queue.add(new ClassWithPrio(source, 0));

        while (!queue.isEmpty()) {
            ClassWithPrio cwp = queue.remove();
            List<ClassWithPrio> next = new ArrayList<>();
            if (target.isAssignableFrom(cwp.clazz.getSuperclass()))
                next.add(new ClassWithPrio(cwp.clazz.getSuperclass(), cwp.prio + 1));
            Arrays.stream(cwp.clazz.getInterfaces()).filter(target::isAssignableFrom).map(clazz -> new ClassWithPrio(clazz, cwp.prio + 1)).forEach(next::add);

            next.forEach(e -> {
                if (e.prio < dist.getOrDefault(e.clazz, Integer.MAX_VALUE)) {
                    dist.put(e.clazz, e.prio);
                    queue.add(e);
                }
            });
        }

        penalty = dist.get(target);

        /*
        findTarget:
        while (target != source) {
            if (target.isAssignableFrom(source.getSuperclass())) {
                source = source.getSuperclass();
                penalty++;
            } else {
                for (Class<?> iface : source.getInterfaces()) {
                    if (target.isAssignableFrom(iface)) {
                        source = iface;
                        penalty++;
                        continue findTarget;
                    }
                }

                throw new AssertionError(
                        "UNREACHABLE: Class " + source.getName() + " was assignable to " +
                                target.getName() + ", but no way of conversion was found");
            }
        } */

        return penalty;
    }
}
