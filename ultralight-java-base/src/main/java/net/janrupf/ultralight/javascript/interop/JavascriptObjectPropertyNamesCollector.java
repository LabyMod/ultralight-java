package net.janrupf.ultralight.javascript.interop;

import net.janrupf.ultralight.annotation.NativeCall;
import net.janrupf.ultralight.annotation.NativeType;
import net.janrupf.ultralight.javascript.JavascriptContext;
import net.janrupf.ultralight.javascript.JavascriptObject;

/**
 * Callback for retrieving names of a Javascript object.
 */
@NativeType("JSObjectGetPropertyNamesCallback")
public interface JavascriptObjectPropertyNamesCollector {
    /**
     * The callback invoked when collecting the names of an object's properties.
     * <p>
     * TODO: Implement copyPropertyNames() Property name collectors are used by {@link
     * JavascriptObject#copyPropertyNames()} and JavaScript for...in loops.
     * <p>
     * A class's {@link JavascriptObjectPropertyNamesCollector} callback only needs to provide the names of properties
     * that the class vends through a custom {@link JavascriptObjectPropertyGetter} or {@link
     * JavascriptObjectPropertySetter}. Other properties, including statically declared properties, properties vended by
     * other classes, and properties belonging to object's prototype, are added independently.
     *
     * @param context The execution context to use
     * @param object  The object whose property names are being collected
     * @return An array of all property names on the given object
     */
    @NativeCall
    String[] collectJavascriptPropertyNames(JavascriptContext context, JavascriptObject object);
}
