package net.labymedia.ultralight.api;

import net.labymedia.ultralight.Databind;
import net.labymedia.ultralight.DatabindJavascriptClass;
import net.labymedia.ultralight.javascript.JavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptObject;

/**
 * Base API for java operations from within javascript.
 */
public final class JavaAPI {
    private final Databind databind;

    /**
     * Constructs a new base API using the given {@link Databind} instance.
     *
     * @param databind The databind instance to use for translating classes
     */
    public JavaAPI(Databind databind) {
        this.databind = databind;
    }

    /**
     * Imports a Java class and makes it available to Javascript.
     *
     * @param context The javascript context to use for converting
     * @param className The fully qualified java class name to import
     * @return The imported java class
     * @throws ClassNotFoundException If the java class can not be found
     */
    @InjectJavascriptContext
    public JavascriptObject importClass(JavascriptContext context, String className) throws ClassNotFoundException {
        Class<?> javaClass = Class.forName(className);

        return context.makeObject(databind.toJavascript(javaClass),
                new DatabindJavascriptClass.Data(null, javaClass));
    }
}
