package net.labymedia.ultralight.api;

import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * Marks the method as accepting a {@link net.labymedia.ultralight.javascript.JavascriptContext} as the <b>first</b>
 * parameter.
 * <p>
 * The {@link net.labymedia.ultralight.Databind} instance will then a call the method with the current context.
 */
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
public @interface InjectJavascriptContext {
}
