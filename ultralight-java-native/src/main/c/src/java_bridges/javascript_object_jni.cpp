/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "ultralight_java/java_bridges/javascript_object_jni.hpp"

#include <JavaScriptCore/JavaScript.h>
#include <tuple>
#include <ultralight_java/ultralight_java_instance.hpp>

#include "ultralight_java/java_bridges/javascript_context_lock_jni.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    static std::tuple<bool, JSObjectRef> extract_secondary(JNIEnv *env, jobject java_instance) {
        auto value = reinterpret_cast<JSObjectRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return {false, nullptr};
        }

        return {true, value};
    }

    static std::tuple<bool, JSContextRef, JSObjectRef> extract(JNIEnv *env, jobject java_instance) {
        auto lock = reinterpret_cast<HoldJavascriptContextLock *>(
            env->CallLongMethod(java_instance, runtime.javascript_locked_object.get_lock_handle_method));
        if(env->ExceptionCheck()) {
            return {false, nullptr, nullptr};
        }

        auto value = reinterpret_cast<JSObjectRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return {false, nullptr, nullptr};
        }

        return {true, lock->get_context(), value};
    }

    static std::tuple<bool, JSContextRef, JSObjectRef, jobject> extract_with_lock(JNIEnv *env, jobject java_instance) {
        jobject lock = env->CallObjectMethod(java_instance, runtime.javascript_locked_object.get_lock_method);
        if(env->ExceptionCheck()) {
            return {false, nullptr, nullptr, nullptr};
        }

        return std::tuple_cat(std::move(extract(env, java_instance)), std::tie(lock));
    }

    jobject JavascriptObjectJNI::get_prototype(JNIEnv *env, jobject java_instance) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSValueRef prototype = JSObjectGetPrototype(context, object);
        JSValueProtect(context, prototype);

        return env->NewObject(
            runtime.javascript_value.clazz,
            runtime.javascript_value.constructor,
            reinterpret_cast<jlong>(prototype),
            lock);
    }

    void JavascriptObjectJNI::set_prototype(JNIEnv *env, jobject java_instance, jobject java_prototype) {
        auto [ok, context, object] = extract(env, java_instance);
        auto [prototype_ok, prototype] = extract_secondary(env, java_prototype);
        if(!ok || !prototype_ok) {
            return;
        }

        JSObjectSetPrototype(context, object, prototype);
    }

    jboolean JavascriptObjectJNI::has_property(JNIEnv *env, jobject java_instance, jstring java_property_name) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        JSStringRef javascript_property_name = Util::create_jsstring_ref_from_jstring(env, java_property_name);
        bool has_property = JSObjectHasProperty(context, value, javascript_property_name);
        JSStringRelease(javascript_property_name);

        return has_property;
    }

    jobject JavascriptObjectJNI::get_property(JNIEnv *env, jobject java_instance, jstring java_property_name) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSStringRef javascript_property_name = Util::create_jsstring_ref_from_jstring(env, java_property_name);
        JSValueRef exception = nullptr;
        JSValueRef property = JSObjectGetProperty(context, object, javascript_property_name, &exception);
        JSStringRelease(javascript_property_name);

        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while retrieving javascript property", context, exception, env, lock);
            return nullptr;
        }

        JSValueProtect(context, property);
        return env->NewObject(
            runtime.javascript_value.clazz,
            runtime.javascript_value.constructor,
            reinterpret_cast<jlong>(property),
            lock);
    }

    void JavascriptObjectJNI::set_property(
        JNIEnv *env, jobject java_instance, jstring java_property_name, jobject java_value, jint attributes) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);
        auto [value_ok, value] = extract_secondary(env, java_value);

        if(!ok || !value_ok) {
            return;
        }

        JSStringRef javascript_property_name = Util::create_jsstring_ref_from_jstring(env, java_property_name);
        JSValueRef exception = nullptr;
        JSObjectSetProperty(context, object, javascript_property_name, value, attributes, &exception);
        JSStringRelease(javascript_property_name);

        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while setting javascript property", context, exception, env, lock);
        }
    }

    jboolean JavascriptObjectJNI::delete_property(JNIEnv *env, jobject java_instance, jstring java_property_name) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);

        if(!ok) {
            return false;
        }

        JSStringRef javascript_property_name = Util::create_jsstring_ref_from_jstring(env, java_property_name);
        JSValueRef exception = nullptr;
        bool did_delete = JSObjectDeleteProperty(context, object, javascript_property_name, &exception);
        JSStringRelease(javascript_property_name);

        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while deleting javascript property", context, exception, env, lock);
            return false;
        }

        return did_delete;
    }

    jboolean JavascriptObjectJNI::has_property_for_key(JNIEnv *env, jobject java_instance, jobject java_property_key) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);
        auto [key_ok, key] = extract_secondary(env, java_property_key);

        if(!ok || !key_ok) {
            return false;
        }

        JSValueRef exception = nullptr;
        bool has_property = JSObjectHasPropertyForKey(context, object, key, &exception);

        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while testing javascript property using key", context, exception, env, lock);
            return false;
        }

        return has_property;
    }

    jobject JavascriptObjectJNI::get_property_for_key(JNIEnv *env, jobject java_instance, jobject java_property_key) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);
        auto [key_ok, key] = extract_secondary(env, java_property_key);

        if(!ok || !key_ok) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        JSValueRef property = JSObjectGetPropertyForKey(context, object, key, &exception);

        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while retrieving javascript property using key", context, exception, env, lock);
            return nullptr;
        }

        JSValueProtect(context, property);
        return env->NewObject(
            runtime.javascript_value.clazz,
            runtime.javascript_value.constructor,
            reinterpret_cast<jlong>(property),
            lock);
    }
    void JavascriptObjectJNI::set_property_for_key(
        JNIEnv *env, jobject java_instance, jobject java_property_key, jobject java_value, jint attributes) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);
        auto [key_ok, key] = extract_secondary(env, java_property_key);
        auto [value_ok, value] = extract_secondary(env, java_value);

        if(!ok || !key_ok || !value_ok) {
            return;
        }

        JSValueRef exception = nullptr;
        JSObjectSetPropertyForKey(context, object, key, value, attributes, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while setting javascript property using key", context, exception, env, lock);
        }
    }

    jboolean JavascriptObjectJNI::delete_property_for_key(
        JNIEnv *env, jobject java_instance, jobject java_property_key) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);
        auto [key_ok, key] = extract_secondary(env, java_property_key);

        if(!ok || !key_ok) {
            return false;
        }

        JSValueRef exception = nullptr;
        bool did_delete = JSObjectDeletePropertyForKey(context, object, key, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while deleting javascript property using key", context, exception, env, lock);
            return false;
        }

        return did_delete;
    }

    jobject JavascriptObjectJNI::get_property_at_index(JNIEnv *env, jobject java_instance, jlong property_index) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);

        if(!ok) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        JSValueRef property = JSObjectGetPropertyAtIndex(context, object, property_index, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while retrieving javascript property using index", context, exception, env, lock);
            return nullptr;
        }

        JSValueProtect(context, property);
        return env->NewObject(
            runtime.javascript_value.clazz,
            runtime.javascript_value.constructor,
            reinterpret_cast<jlong>(property),
            lock);
    }

    void JavascriptObjectJNI::set_property_at_index(
        JNIEnv *env, jobject java_instance, jlong property_index, jobject java_value) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);
        auto [value_ok, value] = extract_secondary(env, java_value);

        if(!ok || !value_ok) {
            return;
        }

        JSValueRef exception = nullptr;
        JSObjectSetPropertyAtIndex(context, object, property_index, value, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while setting javascript property using index", context, exception, env, lock);
        }
    }

    jobject JavascriptObjectJNI::get_private(JNIEnv *env, jobject java_instance) {
        auto [ok, object] = extract_secondary(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        return reinterpret_cast<jobject>(JSObjectGetPrivate(object));
    }

    void JavascriptObjectJNI::set_private(JNIEnv *env, jobject java_instance, jobject java_data) {
        auto [ok, object] = extract_secondary(env, java_instance);
        if(!ok) {
            return;
        }

        auto existing = reinterpret_cast<jobject>(JSObjectGetPrivate(object));
        if(existing && env->GetObjectRefType(existing) != JNIInvalidRefType) {
            env->DeleteGlobalRef(existing);
        }

        JSObjectSetPrivate(object, env->NewGlobalRef(java_data));
    }

    jboolean JavascriptObjectJNI::is_function(JNIEnv *env, jobject java_instance) {
        auto [ok, context, object] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSObjectIsFunction(context, object);
    }

    jobject JavascriptObjectJNI::call_as_function(
        JNIEnv *env, jobject java_instance, jobject java_this_object, jobjectArray java_arguments) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);
        auto [this_object_ok, this_object] = java_this_object ? extract_secondary(env, java_this_object) :
                                                                std::make_tuple(true, nullptr);
        if(!ok || !this_object_ok) {
            return nullptr;
        }

        size_t argument_count = env->GetArrayLength(java_arguments);
        auto *javascript_arguments = Util::translate_bridged_arguments(env, java_arguments);
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        JSValueRef return_value =
            JSObjectCallAsFunction(context, object, this_object, argument_count, javascript_arguments, &exception);
        delete[] javascript_arguments;

        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while calling javascript object as function", context, exception, env, lock);
            return nullptr;
        } else if(!exception && !return_value) {
            env->ThrowNew(runtime.illegal_state_exception.clazz, "Object can't be called as a function");
            return nullptr;
        }

        JSValueProtect(context, return_value);
        return env->NewObject(
            runtime.javascript_value.clazz,
            runtime.javascript_value.constructor,
            reinterpret_cast<jlong>(return_value),
            lock);
    }

    jboolean JavascriptObjectJNI::is_constructor(JNIEnv *env, jobject java_instance) {
        auto [ok, context, object] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSObjectIsConstructor(context, object);
    }

    jobject JavascriptObjectJNI::call_as_constructor(JNIEnv *env, jobject java_instance, jobjectArray java_arguments) {
        auto [ok, context, object, lock] = extract_with_lock(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        size_t argument_count = env->GetArrayLength(java_arguments);
        auto *javascript_arguments = Util::translate_bridged_arguments(env, java_arguments);
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        JSObjectRef
            return_value = JSObjectCallAsConstructor(context, object, argument_count, javascript_arguments, &exception);
        delete[] javascript_arguments;

        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while calling javascript object as constructor", context, exception, env, lock);
            return nullptr;
        }

        JSValueProtect(context, return_value);
        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(return_value),
            lock);
    }

    jobjectArray JavascriptObjectJNI::copy_property_names(JNIEnv *env, jobject java_instance) {
        auto [ok, context, object] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSPropertyNameArrayRef property_names = JSObjectCopyPropertyNames(context, object);
        size_t name_count = JSPropertyNameArrayGetCount(property_names);

        jobjectArray java_property_names = env->NewObjectArray(name_count, runtime.string.clazz, nullptr);

        for(size_t i = 0; i < name_count; i++) {
            JSStringRef property_name = JSPropertyNameArrayGetNameAtIndex(property_names, i);
            jstring java_property_name = Util::create_jstring_from_jsstring_ref(env, property_name);
            env->SetObjectArrayElement(java_property_names, i, java_property_name);
            env->DeleteLocalRef(java_property_name);
        }

        JSPropertyNameArrayRelease(property_names);

        return java_property_names;
    }
} // namespace ultralight_java