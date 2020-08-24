#include "ultralight_java/java_bridges/javascript_class_definition_jni.hpp"

#include <type_traits>

#include "ultralight_java/java_bridges/javascript_class_jni.hpp"
#include "ultralight_java/platform/managed_javascript_class.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

#define DELETE_REF(env, x)                                                                                             \
    if((x))                                                                                                            \
    (env)->DeleteGlobalRef(x)

#define DELETE_ASSIGN(env, t, x)                                                                                       \
    DELETE_REF((env), (t));                                                                                            \
    (t) = reinterpret_cast<std::remove_reference_t<decltype((t))>>((env)->NewGlobalRef((x)))

namespace ultralight_java {
    struct TemporaryClassDescription {
        ManagedJavascriptClassData data;

        int attributes = 0;

        jobject java_parent = nullptr;
        jstring java_name = nullptr;
    };

    jobject JavascriptClassDefinitionJNI::name(JNIEnv *env, jobject java_instance, jstring java_name) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->java_name, java_name);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::attributes(JNIEnv *env, jobject java_instance, jint attributes) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        description->attributes = attributes;
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::parent_class(JNIEnv *env, jobject java_instance, jobject java_parent) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->java_parent, java_parent);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::static_value(
        JNIEnv *env,
        jobject java_instance,
        jstring java_name,
        jobject java_getter,
        jobject java_setter,
        jint attributes) {
        if(!java_setter && !(static_cast<uint32_t>(attributes) & kJSPropertyAttributeReadOnly)) {
            env->ThrowNew(runtime.illegal_argument_exception.clazz, "Read only attribute not set, but setter is null");
            return nullptr;
        } else if(!java_getter) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "getter can't be null");
            return nullptr;
        } else if(!java_name) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "name can't be null");
            return nullptr;
        }

        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        ManagedJavascriptStaticField field;
        field.setter = java_setter ? env->NewGlobalRef(java_setter) : nullptr;
        field.getter = env->NewGlobalRef(java_getter);
        field.attributes = attributes;

        const char *name = env->GetStringUTFChars(java_name, nullptr);
        description->data.static_fields.emplace(name, field);
        env->ReleaseStringUTFChars(java_name, name);

        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::static_function(
        JNIEnv *env, jobject java_instance, jstring java_name, jobject java_function, jint attributes) {
        if(!java_name) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "name can't be null");
            return nullptr;
        } else if(!java_function) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "function can't be null");
            return nullptr;
        }

        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        ManagedJavascriptStaticFunction function;
        function.function = env->NewGlobalRef(java_function);
        function.attributes = attributes;

        const char *name = env->GetStringUTFChars(java_name, nullptr);
        description->data.static_functions.emplace(name, function);
        env->ReleaseStringUTFChars(java_name, name);

        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::on_initialize(JNIEnv *env, jobject java_instance, jobject java_initializer) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->data.functions.java_initializer, java_initializer);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::on_finalize(JNIEnv *env, jobject java_instance, jobject java_finalizer) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->data.functions.java_finalizer, java_finalizer);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::on_get_property(JNIEnv *env, jobject java_instance, jobject java_getter) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->data.functions.java_property_getter, java_getter);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::on_set_property(JNIEnv *env, jobject java_instance, jobject java_setter) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->data.functions.java_property_setter, java_setter);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::on_delete_property(JNIEnv *env, jobject java_instance, jobject java_deleter) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->data.functions.java_property_deleter, java_deleter);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::on_get_property_names(
        JNIEnv *env, jobject java_instance, jobject java_collector) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->data.functions.java_property_name_collector, java_collector);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::on_call_as_function(
        JNIEnv *env, jobject java_instance, jobject java_function) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->data.functions.java_call_as_function, java_function);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::on_call_as_constructor(
        JNIEnv *env, jobject java_instance, jobject java_constructor) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->data.functions.java_call_as_constructor, java_constructor);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::on_has_instance(JNIEnv *env, jobject java_instance, jobject java_tester) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->data.functions.java_has_instance_tester, java_tester);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::on_convert_to_type(
        JNIEnv *env, jobject java_instance, jobject java_converter) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        DELETE_ASSIGN(env, description->data.functions.java_to_type_converter, java_converter);
        return java_instance;
    }

    jobject JavascriptClassDefinitionJNI::bake(JNIEnv *env, jobject java_instance) {
        auto description = reinterpret_cast<TemporaryClassDescription *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        if(!description->java_name) {
            env->ThrowNew(runtime.illegal_state_exception.clazz, "name has not been set");
            return nullptr;
        }

        auto *new_data = new ManagedJavascriptClassData();
        new_data->functions.java_initializer = env->NewGlobalRef(description->data.functions.java_initializer);
        new_data->functions.java_finalizer = env->NewGlobalRef(description->data.functions.java_finalizer);
        new_data->functions.java_has_property_tester = env->NewGlobalRef(
            description->data.functions.java_has_property_tester);
        new_data->functions.java_property_getter = env->NewGlobalRef(description->data.functions.java_property_getter);
        new_data->functions.java_property_setter = env->NewGlobalRef(description->data.functions.java_property_setter);
        new_data->functions.java_property_deleter = env->NewGlobalRef(
            description->data.functions.java_property_deleter);
        new_data->functions.java_property_name_collector = env->NewGlobalRef(
            description->data.functions.java_property_name_collector);
        new_data->functions.java_call_as_function = env->NewGlobalRef(
            description->data.functions.java_call_as_function);
        new_data->functions.java_call_as_constructor = env->NewGlobalRef(
            description->data.functions.java_call_as_constructor);
        new_data->functions.java_has_instance_tester = env->NewGlobalRef(
            description->data.functions.java_has_instance_tester);
        new_data->functions.java_to_type_converter = env->NewGlobalRef(
            description->data.functions.java_to_type_converter);

        new_data->static_functions.reserve(description->data.static_functions.size());

        for(const auto &[key, value] : description->data.static_functions) {
            ManagedJavascriptStaticFunction new_function;
            new_function.function = env->NewGlobalRef(value.function);
            new_function.attributes = value.attributes;

            new_data->static_functions.emplace(key, value);
        }

        new_data->static_fields.reserve(description->data.static_fields.size());

        for(const auto &[key, value] : description->data.static_fields) {
            ManagedJavascriptStaticField new_field;
            new_field.getter = env->NewGlobalRef(value.getter);
            new_field.setter = value.setter ? env->NewGlobalRef(value.setter) : nullptr;
            new_field.attributes = value.attributes;

            new_data->static_fields.emplace(key, new_field);
        }

        JSClassRef parent_class = nullptr;
        if(description->java_parent) {
            parent_class = reinterpret_cast<JSClassRef>(
                env->CallLongMethod(description->java_parent, runtime.object_with_handle.get_handle_method));
            if(env->ExceptionCheck()) {
                return nullptr;
            }
        }

        const char *class_name = env->GetStringUTFChars(description->java_name, nullptr);

        JSClassDefinition class_definition;
        class_definition.version = 1000;
        class_definition.attributes = description->attributes;
        class_definition.className = class_name;
        class_definition.parentClass = parent_class;

        if(!description->data.static_fields.empty()) {
            auto *static_values = new JSStaticValueEx[description->data.static_fields.size() + 1];
            class_definition.staticValuesEx = static_values;

            for(const auto &[name, field] : description->data.static_fields) {
                static_values->name = name.c_str();
                static_values->attributes = field.attributes;
                static_values->getPropertyEx = &ManagedJavascriptCallbacks::get_static_property;

                if(field.setter) {
                    static_values->setPropertyEx = &ManagedJavascriptCallbacks::set_static_property;
                } else {
                    static_values->setPropertyEx = nullptr;
                }

                static_values++;
            }

            static_values->name = nullptr;
            static_values->attributes = 0;
            static_values->setPropertyEx = nullptr;
            static_values->getPropertyEx = nullptr;
        } else {
            class_definition.staticValues = nullptr;
        }

        if(!description->data.static_functions.empty()) {
            auto *static_functions = new JSStaticFunctionEx[description->data.static_functions.size() + 1];
            class_definition.staticFunctionsEx = static_functions;

            for(const auto &[name, function] : description->data.static_functions) {
                static_functions->name = name.c_str();
                static_functions->attributes = function.attributes;
                static_functions->callAsFunctionEx = &ManagedJavascriptCallbacks::call_static_function;

                static_functions++;
            }

            static_functions->name = nullptr;
            static_functions->attributes = 0;
            static_functions->callAsFunctionEx = nullptr;
        } else {
            class_definition.staticFunctions = nullptr;
        }

        class_definition.initializeEx = description->data.functions.java_initializer ?
                                            &ManagedJavascriptCallbacks::initialize :
                                            nullptr;
        class_definition.finalizeEx = &ManagedJavascriptCallbacks::finalize;
        class_definition.hasPropertyEx = description->data.functions.java_has_property_tester ?
                                             &ManagedJavascriptCallbacks::has_property :
                                             nullptr;
        class_definition.getPropertyEx = description->data.functions.java_property_getter ?
                                             &ManagedJavascriptCallbacks::get_property :
                                             nullptr;
        class_definition.setPropertyEx = description->data.functions.java_property_setter ?
                                             &ManagedJavascriptCallbacks::set_property :
                                             nullptr;
        class_definition.deletePropertyEx = description->data.functions.java_property_deleter ?
                                                &ManagedJavascriptCallbacks::delete_property :
                                                nullptr;
        class_definition.getPropertyNamesEx = description->data.functions.java_property_name_collector ?
                                                  &ManagedJavascriptCallbacks::get_property_names :
                                                  nullptr;
        class_definition.callAsFunctionEx = description->data.functions.java_call_as_function ?
                                                &ManagedJavascriptCallbacks::call_as_function :
                                                nullptr;
        class_definition.callAsConstructorEx = description->data.functions.java_call_as_constructor ?
                                                   &ManagedJavascriptCallbacks::call_as_constructor :
                                                   nullptr;
        class_definition.hasInstanceEx = description->data.functions.java_has_instance_tester ?
                                             &ManagedJavascriptCallbacks::has_instance :
                                             nullptr;
        class_definition.convertToTypeEx = description->data.functions.java_to_type_converter ?
                                               &ManagedJavascriptCallbacks::convert_to_type :
                                               nullptr;

        class_definition.privateData = new_data;

        JSClassRef class_ref = JSClassCreate(&class_definition);

        delete[] class_definition.staticFunctions;
        delete[] class_definition.staticValues;

        env->ReleaseStringUTFChars(description->java_name, class_name);

        return env->NewObject(
            runtime.javascript_class.clazz, runtime.javascript_class.constructor, reinterpret_cast<jlong>(class_ref));
    }

    jlong JavascriptClassDefinitionJNI::create_empty(JNIEnv *, jclass) {
        return reinterpret_cast<jlong>(new TemporaryClassDescription());
    }

    void JavascriptClassDefinitionJNI::free(JNIEnv *env, jclass, jlong handle) {
        auto value = reinterpret_cast<TemporaryClassDescription *>(handle);

        DELETE_REF(env, value->data.functions.java_initializer);
        DELETE_REF(env, value->data.functions.java_finalizer);
        DELETE_REF(env, value->data.functions.java_has_property_tester);
        DELETE_REF(env, value->data.functions.java_property_getter);
        DELETE_REF(env, value->data.functions.java_property_setter);
        DELETE_REF(env, value->data.functions.java_property_deleter);
        DELETE_REF(env, value->data.functions.java_property_name_collector);
        DELETE_REF(env, value->data.functions.java_call_as_function);
        DELETE_REF(env, value->data.functions.java_call_as_constructor);
        DELETE_REF(env, value->data.functions.java_has_instance_tester);
        DELETE_REF(env, value->data.functions.java_to_type_converter);

        DELETE_REF(env, value->java_name);

        for(auto &[_, ref] : value->data.static_functions) {
            DELETE_REF(env, ref.function);
        }

        for(auto &[_, field] : value->data.static_fields) {
            DELETE_REF(env, field.getter);
            DELETE_REF(env, field.setter);
        }

        delete value;
    }
} // namespace ultralight_java
