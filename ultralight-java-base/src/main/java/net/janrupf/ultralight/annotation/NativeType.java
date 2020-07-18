package net.janrupf.ultralight.annotation;

import java.lang.annotation.Documented;

/**
 * Annotation for a java type to note which native type it represents.
 */
@Documented
public @interface NativeType {
    /**
     * Returns the fully qualified name of the native type.
     *
     * @return The fully qualified name of the native type
     */
    String value();
}
