package net.janrupf.ultralight.annotation;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Target;

/**
 * Marker annotation to mark fields as a reference holder to prevent unwanted frees.
 */
@Documented
@Target({ElementType.FIELD, ElementType.PARAMETER})
public @interface AntiFree {
}
