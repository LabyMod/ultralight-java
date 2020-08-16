#pragma once

#include <JavaScriptCore/JavaScript.h>
#include <jni.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace ultralight_java {
    /**
     * Helper struct containing operations which Javascript can invoke on a bridged object.
     */
    struct ManagedJavascriptFunctionContainer {
        explicit ManagedJavascriptFunctionContainer();

        jobject java_initializer;
        jobject java_finalizer;
        jobject java_has_property_tester;
        jobject java_property_getter;
        jobject java_property_setter;
        jobject java_property_deleter;
        jobject java_property_name_collector;
        jobject java_call_as_function;
        jobject java_call_as_constructor;
        jobject java_has_instance_tester;
        jobject java_to_type_converter;
    };

    /**
     * Struct containing getter, setter and attributes for a static field
     */
    struct ManagedJavascriptStaticField {
        explicit ManagedJavascriptStaticField();

        jobject getter;
        jobject setter;
        int attributes;
    };

    /**
     * Struct containing function and attributes for a static function
     */
    struct ManagedJavascriptStaticFunction {
        explicit ManagedJavascriptStaticFunction();

        jobject function;
        int attributes;
    };

    /**
     * Struct used as private data for Javascript objects with classes from java
     */
    struct ManagedJavascriptClassData {
        explicit ManagedJavascriptClassData();

        /**
         * The callbacks used for this object
         */
        ManagedJavascriptFunctionContainer functions;

        /**
         * Bridged static fields
         */
        std::unordered_map<std::string, ManagedJavascriptStaticField> static_fields;

        /**
         * Bridged static functions
         */
        std::unordered_map<std::string, ManagedJavascriptStaticFunction> static_functions;
    };

    /**
     * Class containing the callbacks used by the Javascript engine to connect back to Java.
     */
    class ManagedJavascriptCallbacks {
    public:
        /**
         * Initializes a Javascript object bridged from Java. The private data of the object needs to point
         * to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param object The object to initialize
         */
        static void initialize(JSContextRef ctx, JSClassRef clazz, JSObjectRef object);

        /**
         * Finalizes a Javascript object bridged from Java. The private data of the object needs to point
         * to a ManagedJavascriptPrivateData struct.
         *
         * @param clazz The class the callback is being invoked for
         * @param object The object to finalize
         */
        static void finalize(JSClassRef clazz, JSObjectRef object);

        /**
         * Tests whether a Javascript object bridged from Java has a certain property. The private data of the object
         * needs to point to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param object The object to search for the property
         * @param property_name The name of the property to search for
         * @return true if the object has the requested property, false otherwise
         */
        static bool has_property(JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSStringRef property_name);

        /**
         * Retrieves a property from a Javascript object bridged from Java. The private data of the object needs to
         * point to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param object The object to retrieve the property from
         * @param property_name The name of the property to retrieve
         * @param exception Pointer to store an exception into if one occurs
         * @return The found property, or null, if the request shall be forwarded
         */
        static JSValueRef get_property(
            JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSStringRef property_name, JSValueRef *exception);

        /**
         * Retrieves a static property from a Javascript object bridged from Java. The private data of the object needs
         * to point to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param object The object to retrieve the static property from
         * @param property_name The name of the static property to retrieve
         * @param exception Pointer to store an exception into if one occurs
         * @return The found static property, or null, if the request shall be forwarded
         */
        static JSValueRef get_static_property(
            JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSStringRef property_name, JSValueRef *exception);

        /**
         * Sets a property on a Javascript object bridged from Java. The private data of the object needs to point
         * to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param object The object to set the property on
         * @param property_name The name of the property to set
         * @param value The value to set the property to
         * @param exception Pointer to store an exception into if one occurs
         * @return true if the property has been set or an exception occurred, false if setting the property should be
         *         delegated
         */
        static bool set_property(
            JSContextRef ctx,
            JSClassRef clazz,
            JSObjectRef object,
            JSStringRef property_name,
            JSValueRef value,
            JSValueRef *exception);

        /**
         * Sets a static property on a Javascript object bridged from Java. The private data of the object needs to point
         * to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param object The object to set the static property on
         * @param property_name The name of the static property to set
         * @param value The value to set the static property to
         * @param exception Pointer to store an exception into if one occurs
         * @return true if the property has been set or an exception occurred, false if setting the property should be
         *         delegated
         */
        static bool set_static_property(
            JSContextRef ctx,
            JSClassRef clazz,
            JSObjectRef object,
            JSStringRef property_name,
            JSValueRef value,
            JSValueRef *exception);

        /**
         * Deletes a property from a Javascript object bridged from Java. The private data of the object needs to point
         * to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param object The object to delete the property from
         * @param property_name The name of the property to delete
         * @param exception Pointer to store an exception into if one occurs
         * @return true if the property has been deleted or an exception occurred, false if deleting the property should
         *         be delegated
         */
        static bool delete_property(
            JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSStringRef property_name, JSValueRef *exception);

        /**
         * Collects the names of a Javascript object bridged from Java. The private data of the object needs to point
         * to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param object The object to collect the property names from
         * @param property_names The accumulator to store the names into
         */
        static void get_property_names(
            JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSPropertyNameAccumulatorRef property_names);

        /**
         * Calls a Javascript object bridged from Java as a function. The private data of the object needs to point
         * to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param function The function being called
         * @param function_name The name of the function being called
         * @param this_object The this context to use while calling the function
         * @param argument_count The amount of arguments in the arguments array
         * @param arguments The arguments the function is being called with
         * @param exception Pointer to store an exception into if one occurs
         * @return The object returned by the function
         */
        static JSValueRef call_as_function(
            JSContextRef ctx,
            JSClassRef clazz,
            JSStringRef function_name,
            JSObjectRef function,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef *exception);

        /**
         * Calls a static function on a Javascript object bridged from Java. The private data of the object needs to point
         * to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param function The function being called
         * @param function_name The name of the function being called
         * @param this_object The this context to use while calling the function
         * @param argument_count The amount of arguments in the arguments array
         * @param arguments The arguments the function is being called with
         * @param exception Pointer to store an exception into if one occurs
         * @return The object returned by the function
         */
        static JSValueRef call_static_function(
            JSContextRef ctx,
            JSClassRef clazz,
            JSStringRef function_name,
            JSObjectRef function,
            JSObjectRef this_object,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef *exception);

        /**
         * Calls a Javascript object bridged from Java as a constructor. The private data of the object needs to point
         * to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param constructor The constructor being called
         * @param argument_count The amount of arguments in the arguments array
         * @param arguments The arguments the constructor is being called with
         * @param exception Pointer to store an exception into if one occurs
         * @return The constructed object
         */
        static JSObjectRef call_as_constructor(
            JSContextRef ctx,
            JSClassRef clazz,
            JSObjectRef constructor,
            size_t argument_count,
            const JSValueRef arguments[],
            JSValueRef *exception);

        /**
         * Tests whether a Javascript object bridged from Java is an instance of another object. The private data of the
         * object needs to point to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param constructor The constructor to use as the base
         * @param possible_instance The object which possibly is an instance of the constructor
         * @param exception Pointer to store an exception into if one occurs
         * @return true if the object is an instance of the constructor, false otherwise
         */
        static bool has_instance(
            JSContextRef ctx,
            JSClassRef clazz,
            JSObjectRef constructor,
            JSValueRef possible_instance,
            JSValueRef *exception);

        /**
         * Converts a Javascript object bridged from Java to another type. The private data of the object
         * needs to point to a ManagedJavascriptPrivateData struct.
         *
         * @param ctx The execution context to use
         * @param clazz The class the callback is being invoked for
         * @param object The object to convert
         * @param type The type to convert the object into
         * @param exception Pointer to store an exception into if one occurs
         * @return The converted object
         */
        static JSValueRef convert_to_type(
            JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSType type, JSValueRef *exception);
    };
} // namespace ultralight_java