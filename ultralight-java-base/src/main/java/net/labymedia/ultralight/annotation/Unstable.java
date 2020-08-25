package net.labymedia.ultralight.annotation;

import java.lang.annotation.Documented;

/**
 * Marks an element as subject to change. This might be used when it is not yet clear if an implementation will be kept
 * or changed.
 */
@Documented
public @interface Unstable {
    /**
     * Specifies the reason this element has been marked as unstable.
     *
     * @return The reason this element is unstable
     */
    String value();
}
