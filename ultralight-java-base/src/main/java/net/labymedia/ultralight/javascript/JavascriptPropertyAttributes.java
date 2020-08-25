package net.labymedia.ultralight.javascript;

/**
 * Attributes of properties on Javascript classes. Defines how those properties can be altered.
 */
public final class JavascriptPropertyAttributes {
    /**
     * Specifies that a property has no special attributes.
     */
    public static int NONE = 0;

    /**
     * Specifies that a property is read-only.
     */
    public static int READ_ONLY = 1 << 1;

    /**
     * <p>
     * Specifies that a property should not be enumerated by {@link JavascriptObject#copyPropertyNames()}
     * and JavaScript for...in loops.
     */
    public static int DONT_ENUM = 1 << 2;

    /**
     * Specifies that the delete operation should fail on a property.
     */
    public static int DONT_DELETE = 1 << 3;
}
