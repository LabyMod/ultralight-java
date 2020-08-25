package net.labymedia.ultralight.javascript;

import net.labymedia.ultralight.annotation.NativeType;

import java.lang.annotation.Native;

/**
 * A constant identifying the type of a {@link JavascriptValue}.
 */
@NativeType("JSType")
public enum JavascriptType {
    /**
     * The unique undefined value.
     */
    @Native
    UNDEFINED,

    /**
     * The unique null value.
     */
    @Native
    NULL,

    /**
     * A primitive boolean value, one of {@code true} or false {@code false}.
     */
    @Native
    BOOLEAN,

    /**
     * A primitive number value.
     */
    @Native
    NUMBER,

    /**
     * A primitive string value.
     */
    @Native
    STRING,

    /**
     * An object value (meaning that the {@link JavascriptValue} is a {@link JavascriptObject}).
     */
    @Native
    OBJECT,

    /**
     * A primitive symbol value.
     */
    @Native
    SYMBOL
}
