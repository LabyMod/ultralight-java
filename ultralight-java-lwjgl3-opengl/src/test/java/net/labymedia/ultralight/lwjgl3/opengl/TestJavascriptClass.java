package net.labymedia.ultralight.lwjgl3.opengl;

import net.labymedia.ultralight.javascript.*;

import java.util.Arrays;

public class TestJavascriptClass {
    public static final JavascriptClass INSTANCE = new JavascriptClassDefinition()
            .name(TestJavascriptClass.class.getName())
            .staticValue("someValue", TestJavascriptClass::getStaticValue, TestJavascriptClass::setStaticValue, 0)
            .staticValue("someOtherValue", TestJavascriptClass::getStaticValue, TestJavascriptClass::setStaticValue, 0)
            .staticFunction("run", TestJavascriptClass::staticFunction, 0)
            .onSetProperty(TestJavascriptClass::setValue)
            .onGetProperty(TestJavascriptClass::getValue)
            .onDeleteProperty(TestJavascriptClass::deleteProperty)
            .onGetPropertyNames(TestJavascriptClass::getPropertyNames)
            .onCallAsFunction(TestJavascriptClass::onCallAsFunction)
            .onCallAsConstructor(TestJavascriptClass::onCallAsConstructor)
            .onHasInstance(TestJavascriptClass::hasInstance)
            .onConvertToType(TestJavascriptClass::toType)
            .bake();

    private static boolean setStaticValue(JavascriptContext context, JavascriptObject object, String name, JavascriptValue newValue) {
        System.out.println("Setting static value " + name + " to " + newValue + " on " + object);
        return true;
    }

    private static JavascriptValue getStaticValue(JavascriptContext context, JavascriptObject object, String name) {
        System.out.println("Retrieving static value " + name + " from " + object);
        return context.makeString("Some static value");
    }

    private static JavascriptValue staticFunction(
            JavascriptContext context,
            JavascriptObject thisObject,
            JavascriptObject function,
            JavascriptValue[] arguments
    ) {
        System.out.println("Called static function " + function + " on " + thisObject);
        return context.makeString("Return value from function");
    }

    private static boolean setValue(JavascriptContext context, JavascriptObject object, String name, JavascriptValue newValue) {
        System.out.println("Setting value " + name + " to " + newValue + " on " + object);
        return true;
    }

    private static JavascriptValue getValue(JavascriptContext context, JavascriptObject object, String name) {
        System.out.println("Retrieving value " + name + " from " + object);
        return context.makeString("Some value");
    }

    private static boolean deleteProperty(JavascriptContext context, JavascriptObject object, String name) {
        System.out.println("Deleting property " + name + " from " + object);
        return true;
    }

    private static String[] getPropertyNames(JavascriptContext context, JavascriptObject object) {
        return new String[]{"someValue", "someOtherValue"};
    }

    private static JavascriptValue onCallAsFunction(
            JavascriptContext context,
            JavascriptObject thisObject,
            JavascriptObject function,
            JavascriptValue[] arguments
    ) {
        System.out.println("Called function " + function + " on " + thisObject + " with parameters " + Arrays.toString(arguments));
        return context.makeString("Function return value");
    }

    private static JavascriptObject onCallAsConstructor(
            JavascriptContext context,
            JavascriptObject constructor,
            JavascriptValue[] arguments
    ) {
        System.out.println("Called " + constructor + " as a constructor with parameters " + Arrays.toString(arguments));
        return context.makeFromJSON("{}").toObject();
    }

    private static boolean hasInstance(JavascriptContext context, JavascriptObject constructor, JavascriptValue possibleInstance) {
        System.out.println("Testing if " + possibleInstance + " is an instance of " + constructor);
        return false;
    }

    private static JavascriptValue toType(JavascriptContext context, JavascriptObject object, JavascriptType type) {
        System.out.println("Converting " + object + " to " + type.name());
        switch (type) {
            case STRING:
                return context.makeString(TestJavascriptClass.class.getName() + " converted to string");

            case NUMBER:
                return context.makeNumber(42);

            default:
                throw new UnsupportedOperationException("Can only convert to string or number");
        }
    }
}
