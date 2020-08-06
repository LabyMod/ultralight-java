package net.janrupf.ultralight.javascript;

import net.janrupf.ultralight.annotation.NativeCall;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.annotation.Unstable;
import net.janrupf.ultralight.ffi.ObjectWithHandle;

import java.util.Objects;

/**
 * A JavaScript value. The base type for all JavaScript values, and polymorphic functions on them.
 */
@NativeType("JSValueRef")
public class JavascriptValue implements ObjectWithHandle, JavascriptLockedObject {
    private final long handle;
    private final JavascriptContextLock lock;

    /**
     * Constructs a new {@link JavascriptValue} wrapping an existing value reference in a locked context.
     *
     * @param handle The native value reference
     * @param lock   The context the value exists in, or {@code null}, if the object is not locked
     */
    @NativeCall
    JavascriptValue(@NativeType("JSValueRef") long handle, JavascriptContextLock lock) {
        this.handle = handle;
        this.lock = lock;

        if(this.lock != null) {
            this.lock.addDependency(this);
        }
    }

    /**
     * Protects this value and creates a copy which survives outside of the lock.
     *
     * @return The protected value
     */
    public native JavascriptProtectedValue protect();

    /**
     * Retrieves the type of this value.
     *
     * @return The type of this value
     */
    public native JavascriptType getType();

    /**
     * Tests whether this value is of the {@link JavascriptType#UNDEFINED} type.
     *
     * @return {@code true} if this value is an undefined value, {@code false} otherwise
     */
    public native boolean isUndefined();

    /**
     * Tests whether this value is of the {@link JavascriptType#NULL} type.
     *
     * @return {@code true} if this value is a null value, {@code false} otherwise
     */
    public native boolean isNull();

    /**
     * Tests whether this value is of the {@link JavascriptType#BOOLEAN} type.
     *
     * @return {@code true} if this value is a boolean value, {@code false} otherwise
     */
    public native boolean isBoolean();

    /**
     * Tests whether this value is of the {@link JavascriptType#NUMBER} type.
     *
     * @return {@code true} if this value is a number value, {@code false} otherwise
     */
    public native boolean isNumber();

    /**
     * Tests whether this value is of the {@link JavascriptType#STRING} type.
     *
     * @return {@code true} if this value is a string value, {@code false} otherwise
     */
    public native boolean isString();

    /**
     * Tests whether this value is of the {@link JavascriptType#SYMBOL} type.
     *
     * @return {@code true} if this value is a symbol value, {@code false} otherwise
     */
    public native boolean isSymbol();

    /**
     * Tests whether this value is of the {@link JavascriptType#OBJECT} type.
     *
     * @return {@code true} if this value is an object value, {@code false} otherwise
     */
    public native boolean isObject();

    // TODO: isObjectOfClass

    /**
     * Tests whether this value is of the {@link JavascriptType#OBJECT} type and is an array.
     *
     * @return {@code true} if this value is an array value, {@code false} otherwise
     */
    public native boolean isArray();

    /**
     * Tests whether this value is of the {@link JavascriptType#OBJECT} type and is a date.
     *
     * @return {@code true} if this value is a date value, {@code false} otherwise
     */
    public native boolean isDate();

    /**
     * Retrieves the {@link JavascriptTypedArrayType} of this value.
     *
     * @return The typed array type of this value
     * @throws JavascriptException If retrieving the type throws an exception
     */
    public native JavascriptTypedArrayType getTypedArrayType();

    /**
     * Tests whether this {@link JavascriptValue} is equal to another one, works like the == operator in Javascript.
     * <p>
     * Passing {@code null} as the other value will result in the value being equal, if this value is a {@link
     * JavascriptType#NULL} or {@link JavascriptType#UNDEFINED} value
     *
     * @param other The value to compare this value with
     * @return {@code true} if the value is equal to the other one, {@code false} otherwise
     * @throws JavascriptException If an exception occurs while checking if the values are equal
     */
    public native boolean isEqual(JavascriptValue other);

    /**
     * Tests whether this {@link JavascriptValue} is strictly equal to another one, works like the === operator in
     * Javascript.
     * <p>
     * Passing {@code null} as the other value will result in the value being equal, if this value is a {@link
     * JavascriptType#NULL} value.
     *
     * @param other The value to compare this value with
     * @return {@code true} if the value is strictly equal to the other one, {@code false} otherwise
     */
    public native boolean isStrictEqual(JavascriptValue other);

    /**
     * Special equals method for {@link JavascriptValue}s. <b>This method only compares the handle, use the {@link
     * #isEqual(JavascriptValue)} or {@link #isStrictEqual(JavascriptValue)} methods to check for equality.</b>
     * <p>
     * As per Java standard, this method does not make {@code null} equal with {@link JavascriptType#NULL} or {@link
     * JavascriptType#UNDEFINED}, for methods following the javascript semantics, use {@link #isEqual(JavascriptValue)}
     * or {@link #isStrictEqual(JavascriptValue)}.
     * <p>
     * <b>This method is unstable!</b>
     *
     * @param other The object to compare this value with
     * @return {@code true} if the other object is also a {@link JavascriptValue} and has the same handle, {@code false}
     *         otherwise
     */
    @Unstable("Comparison of Javascript objects is subject to change")
    public boolean equals(Object other) {
        if(!(other instanceof JavascriptValue)) {
            return false;
        }

        return ((JavascriptValue) other).handle == handle;
    }

    // TODO: isInstanceOfConstructor

    /**
     * Converts this {@link JavascriptValue} into a JSON string.
     *
     * @param indentation Amount of spaces to use for indentation, clamped to 10
     * @return This value as a JSON string
     * @throws JavascriptException If the conversion to a JSON string fails
     */
    public native String toJson(short indentation);

    /**
     * Converts this {@link JavascriptValue} to a boolean.
     *
     * @return The boolean representation of this value
     */
    public native boolean toBoolean();

    /**
     * Converts this {@link JavascriptValue} to a number.
     *
     * @return The double representation of this value
     * @throws JavascriptException If the conversion to a number fails
     */
    public native double toNumber();

    /**
     * Copies the string representation of this value.
     *
     * @return The string representation of this value
     * @throws JavascriptException If the conversion to a string fails
     */
    public native String toStringCopy();

    // TODO: toObject()

    @Override
    public String toString() {
        if(lock != null && lock.isLocked()) {
            try {
                return toStringCopy();
            } catch(JavascriptException ignored) {
                return "JavascriptValue{handle = " + handle + ", lock = " + lock.getHandle() + "}";
            }
        } else {
            return "JavascriptValue{handle = " + handle + ", lock = none}";
        }
    }

    @Override
    public int hashCode() {
        return Objects.hashCode(handle);
    }

    @Override
    public long getHandle() {
        if(lock == null) {
            throw new IllegalStateException("This value has never been locked");
        } if(!lock.isLocked()) {
            throw new IllegalStateException("JavascriptContext is not locked anymore");
        }

        return handle;
    }

    @Override
    public long getContextHandle() {
        if(lock == null) {
            throw new IllegalStateException("This value has never been locked");
        }if(!lock.isLocked()) {
            throw new IllegalStateException("JavascriptContext is not locked anymore");
        }

        return lock.getContext().getHandle();
    }

    /**
     * Releases the internal reference.
     */
    @Override
    public native void contextUnlocking();

    @Override
    public JavascriptContextLock getLock() {
        return lock;
    }
}
