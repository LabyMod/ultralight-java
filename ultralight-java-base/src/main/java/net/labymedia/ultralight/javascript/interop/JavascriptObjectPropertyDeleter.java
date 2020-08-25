package net.labymedia.ultralight.javascript.interop;

import net.labymedia.ultralight.annotation.NativeCall;
import net.labymedia.ultralight.annotation.NativeType;
import net.labymedia.ultralight.javascript.JavascriptContext;
import net.labymedia.ultralight.javascript.JavascriptObject;

/**
 * Callback for deleting properties of Javascript objects.
 */
@NativeType("JSObjectDeletePropertyCallback")
public interface JavascriptObjectPropertyDeleter {
    /**
     * The callback invoked when deleting a property.
     * <p>
     * If this function returns false, the delete request forwards to object's statically declared properties, then its
     * parent class chain (which includes the default object class).
     *
     * @param context      The execution context to use
     * @param object       The object in which to delete the property
     * @param propertyName A string containing the name of the property to delete
     * @return {@code true} if propertyName was successfully deleted, otherwise {@code false}
     * @throws JavascriptInteropException If an error occurs while deleting the property
     */
    @NativeCall
    boolean deleteJavascriptProperty(JavascriptContext context, JavascriptObject object, String propertyName)
            throws JavascriptInteropException;
}
