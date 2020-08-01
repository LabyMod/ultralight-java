#pragma once

#include <jni.h>

namespace ultralight_java {
    /**
     * Class for interfacing with JSClassDefinition from Java
     */
    class JavascriptClassDefinitionJNI {
    public:
        /**
         * Sets the class name of this definition.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_name The name to set
         * @return this
         */
        static jobject name(JNIEnv *env, jobject java_instance, jstring java_name);

        /**
         * Sets the attributes of this definition.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param attributes The attributes to set
         * @return this
         */
        static jobject attributes(JNIEnv *env, jobject java_instance, jint attributes);

        /**
         * Sets the parent class of this definition.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_parent the parent to set
         * @return this
         */
        static jobject parent_class(JNIEnv *env, jobject java_instance, jobject java_parent);

        /**
         * Adds a static value to this definition.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_name The name of the value to add
         * @param java_getter The callback to use for retrieving the value
         * @param java_setter The callback to use for setting the value, or nullptr
         * @param attributes The attributes of the value
         * @return this
         */
        static jobject static_value(
            JNIEnv *env,
            jobject java_instance,
            jstring java_name,
            jobject java_getter,
            jobject java_setter,
            jint attributes);

        /**
         * Adds a static function to this definition.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_name The name of the function to add
         * @param java_function The function to add
         * @param attributes The attributes of the function
         * @return this
         */
        static jobject static_function(
            JNIEnv *env, jobject java_instance, jstring java_name, jobject java_function, jint attributes);


        /**
         * Sets the initializer of this definition.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_initializer The initializer to set
         * @return this
         */
        static jobject on_initialize(JNIEnv *env, jobject java_instance, jobject java_initializer);

        /**
         * Sets the finalizer of this definition.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_finalizer The finalizer to set
         * @return this
         */
        static jobject on_finalize(JNIEnv *env, jobject java_instance, jobject java_finalizer);

        /**
         * Sets the callback to invoke when retrieving properties.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_getter The getter to set
         * @return this
         */
        static jobject on_get_property(JNIEnv *env, jobject java_instance, jobject java_getter);

        /**
         * Sets the callback to invoke when setting properties.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_setter The setter to set
         * @return this
         */
        static jobject on_set_property(JNIEnv *env, jobject java_instance, jobject java_setter);

        /**
         * Sets the callback to invoke when deleting properties.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_deleter The deleter to set
         * @return this
         */
        static jobject on_delete_property(JNIEnv *env, jobject java_instance, jobject java_deleter);

        /**
         * Sets the callback to invoke to when collecting the names of the properties.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_collector The collector to set
         * @return this
         */
        static jobject on_get_property_names(JNIEnv *env, jobject java_instance, jobject java_collector);

        /**
         * Sets the callback to invoke when invoking the object as a function.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_function The function to set
         * @return this
         */
        static jobject on_call_as_function(JNIEnv *env, jobject java_instance, jobject java_function);

        /**
         * Sets the callback to invoke when constructing the object using the new operator.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_constructor The constructor to set
         * @return this
         */
        static jobject on_call_as_constructor(JNIEnv *env, jobject java_instance, jobject java_constructor);

        /**
         * Sets the callback to invoke when performing an instance test using the instanceof operator.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_tester The tester to set
         * @return this
         */
        static jobject on_has_instance(JNIEnv *env, jobject java_instance, jobject java_tester);

        /**
         * Sets the callback to invoke when converting the object to another type.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_converter The converter to set
         * @return this
         */
        static jobject on_convert_to_type(JNIEnv *env, jobject java_instance, jobject java_converter);

        /**
         * Allocates an empty definition.
         *
         * @param env The JNI environment to use for accessing java
         * @param caller_class The class calling this method, should always be JavascriptClassDefinition
         * @return A pointer to an empty definition
         */
        static jlong create_empty(JNIEnv *env, jclass caller_class);

        /**
         * Free's a class definition.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this JSClassDefinition
         * @param java_name The name to set
         * @return this
         */
        static void free(JNIEnv *env, jclass caller_class, jlong handle);
    };
} // namespace ultralight_java