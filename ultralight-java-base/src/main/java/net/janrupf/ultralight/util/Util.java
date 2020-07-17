package net.janrupf.ultralight.util;

/**
 * General purpose utility class containing methods which don't fit anywhere else.
 */
public class Util {
    // Static class
    private Util() {}

    /**
     * Method to simply force cast any type to any type. Use with caution,
     * absolutely no checks are done. This is often useful when generic signatures
     * begin to clash and you can ensure that it is safe to erase them.
     *
     * @param o The object to cast
     * @param <T> The target type
     * @return The casted object
     */
    @SuppressWarnings("unchecked")
    public static <T> T forceCast(Object o) {
        return (T) o;
    }
}
