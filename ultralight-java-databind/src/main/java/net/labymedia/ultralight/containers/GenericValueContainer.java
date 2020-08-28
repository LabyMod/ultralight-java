package net.labymedia.ultralight.containers;

public final class GenericValueContainer<T> {
    private final T value;

    public GenericValueContainer(final T value) {
        this.value = value;
    }

    public T value() {
        return value;
    }
}
