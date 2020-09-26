package net.labymedia.ultralight.utils;

import java.lang.reflect.Executable;
import java.util.Collection;

public final class HeuristicMethodChooser implements MethodChooser {
    @Override
    public Executable choose(Collection<? extends Executable> possibilities, Class<?>... sourceParameterTypes) {
        int lowestPenalty = Integer.MAX_VALUE;
        Executable mostProbableOption = null;

        for (Executable possibility : possibilities) {
            Class<?>[] targetParameterTypes = possibility.getParameterTypes();
            int penalty;

            if (possibility.isVarArgs()) {
                penalty = calculateVarArgsPenalty(targetParameterTypes, sourceParameterTypes);
            } else if (targetParameterTypes.length != sourceParameterTypes.length) {
                penalty = Integer.MAX_VALUE;
            } else {
                penalty = calculatePenalty(targetParameterTypes, sourceParameterTypes);
            }

            if (penalty < lowestPenalty) {
                lowestPenalty = penalty;
                mostProbableOption = possibility;
            }
        }

        return mostProbableOption;
    }

    private int calculateVarArgsPenalty(Class<?>[] targetParameterTypes, Class<?>... sourceParameterTypes) {
        if (sourceParameterTypes.length < targetParameterTypes.length - 1) {
            return Integer.MAX_VALUE;
        } else if (sourceParameterTypes.length == targetParameterTypes.length - 1) {
            return calculatePenalty(targetParameterTypes, sourceParameterTypes);
        }

        int penality = 0;

        for (int i = 0; i < sourceParameterTypes.length; i++) {
            if (i >= targetParameterTypes.length - 1) {
                if (sourceParameterTypes[i] != targetParameterTypes[targetParameterTypes.length - 1].getComponentType()) {
                    penality = Integer.MAX_VALUE;
                    break;
                }
            } else {
                penality += calculatePenalty(targetParameterTypes[i], sourceParameterTypes[i]);
            }
        }

        return penality;
    }

    private int calculatePenalty(Class<?>[] targetParameterTypes, Class<?>... sourceParameterTypes) {
        int penalty = 0;

        for (int i = 0; i < sourceParameterTypes.length; i++) {
            penalty += calculatePenalty(targetParameterTypes[i], sourceParameterTypes[i]);
        }

        return penalty;
    }

    private int calculatePenalty(Class<?> target, Class<?> source) {
        if (!target.isAssignableFrom(source)) {
            return Integer.MAX_VALUE;
        }

        int penalty = 0;

        while (source != target) {
            Class<?> superSource = source.getSuperclass();

            if (!target.isAssignableFrom(superSource)) {
                superSource = null;

                for (Class<?> interfaceSource : source.getInterfaces()) {
                    if (target.isAssignableFrom(interfaceSource)) {
                        superSource = interfaceSource;
                        break;
                    }
                }

                if (superSource == null) {
                    throw new IllegalStateException(source.getName() + " was assignable to " + target.getName() + ", but no conversion has been found");
                }
            }

            source = superSource;
            penalty++;
        }

        return penalty;
    }
}
