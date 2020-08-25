package net.labymedia.ultralight.javascript;

import net.labymedia.ultralight.annotation.NativeType;

/**
 * Attributes of a Javascript class. Defines how the Javascript class is created.
 */
public final class JavascriptClassAttributes {
    /**
     * Specifies that a class has no special attributes.
     */
    @NativeType("<unnamed enum>")
    public static final int NONE = 0;

    /**
     * Specifies that a class should not automatically generate a shared prototype for its instance objects. Use
     * this in combination with {@link JavascriptObject#setPrototype(JavascriptValue)} to manage prototypes manually.
     */
    @NativeType("<unnamed enum>")
    public static final int NO_AUTOMATIC_PROTOTYPE = 1 << 1;
}
