package net.janrupf.ultralight.annotation;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Target;

/**
 * Marks a method or constructor as called by native code.
 */
@Documented
@Target({ElementType.CONSTRUCTOR, ElementType.METHOD})
public @interface NativeCall {
}
