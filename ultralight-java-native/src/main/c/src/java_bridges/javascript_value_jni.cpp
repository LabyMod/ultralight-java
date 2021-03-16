/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2021 LabyMedia and contributors
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "ultralight_java/java_bridges/javascript_value_jni.hpp"

#include <JavaScriptCore/JavaScript.h>
#include <ultralight_java/ultralight_java_instance.hpp>

#include "ultralight_java/java_bridges/javascript_context_lock_jni.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    static std::tuple<bool, JSContextRef, JSValueRef> extract(JNIEnv *env, jobject java_instance) {
        auto lock = reinterpret_cast<HoldJavascriptContextLock *>(
            env->CallLongMethod(java_instance, runtime.javascript_locked_object.get_lock_handle_method));
        if(env->ExceptionCheck()) {
            return {false, nullptr, nullptr};
        }

        auto value = reinterpret_cast<JSValueRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return {false, nullptr, nullptr};
        }

        return {true, lock->get_context(), value};
    }

    static std::tuple<bool, JSValueRef> extract_secondary(JNIEnv *env, jobject java_instance) {
        auto value = reinterpret_cast<JSValueRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return {false, nullptr};
        }

        return {true, value};
    }

    static std::tuple<bool, JSContextRef, JSValueRef, jobject> extract_with_lock(JNIEnv *env, jobject java_instance) {
        jobject lock = env->CallObjectMethod(java_instance, runtime.javascript_locked_object.get_lock_method);
        if(env->ExceptionCheck()) {
            return {false, nullptr, nullptr, nullptr};
        }

        return std::tuple_cat(std::move(extract(env, java_instance)), std::tie(lock));
    }

    jobject JavascriptValueJNI::protect(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSValueProtect(context, value);

        return env->NewObject(
            runtime.javascript_protected_value.clazz,
            runtime.javascript_protected_value.constructor,
            reinterpret_cast<jlong>(value));
    }

    void JavascriptValueJNI::context_unlocking(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return;
        }

        JSValueUnprotect(context, value);
    }

    jobject JavascriptValueJNI::get_type(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        auto type = JSValueGetType(context, value);
        return runtime.javascript_type.constants.to_java(env, type);
    }

    bool JavascriptValueJNI::is_undefined(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSValueIsUndefined(context, value);
    }

    bool JavascriptValueJNI::is_null(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSValueIsNull(context, value);
    }

    bool JavascriptValueJNI::is_boolean(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSValueIsBoolean(context, value);
    }

    bool JavascriptValueJNI::is_number(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSValueIsNumber(context, value);
    }

    bool JavascriptValueJNI::is_string(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSValueIsString(context, value);
    }

    bool JavascriptValueJNI::is_symbol(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSValueIsSymbol(context, value);
    }

    bool JavascriptValueJNI::is_object(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSValueIsObject(context, value);
    }

    bool JavascriptValueJNI::is_of_class(JNIEnv *env, jobject java_instance, jobject java_class) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        auto clazz = reinterpret_cast<JSClassRef>(
            env->CallLongMethod(java_class, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return false;
        }

        return JSValueIsObjectOfClass(context, value, clazz);
    }

    bool JavascriptValueJNI::is_array(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSValueIsArray(context, value);
    }

    bool JavascriptValueJNI::is_date(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSValueIsDate(context, value);
    }

    jobject JavascriptValueJNI::get_typed_array_type(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value, lock] = extract_with_lock(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        auto typed_array_type = JSValueGetTypedArrayType(context, value, &exception);

        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Exception while retrieving typed array type", context, exception, env, lock);
            return nullptr;
        }

        return runtime.javascript_typed_array_type.constants.to_java(env, typed_array_type);
    }

    bool JavascriptValueJNI::is_equal(JNIEnv *env, jobject java_instance, jobject java_other) {
        auto [ok, context, value, lock] = extract_with_lock(env, java_instance);
        if(!ok) {
            return false;
        }

        if(!java_other) {
            return JSValueIsNull(context, value) || JSValueIsUndefined(context, value);
        }

        auto [other_ok, other_context, other_value] = extract(env, java_other);
        if(!other_ok) {
            return false;
        }

        JSValueRef exception = nullptr;
        bool are_equal = JSValueIsEqual(context, value, other_value, &exception);
        if(exception) {
            Util::
                throw_jssvalue_ref_as_java_exception("Exception while comparing values", context, exception, env, lock);
            return false;
        }

        return are_equal;
    }

    bool JavascriptValueJNI::is_strict_equal(JNIEnv *env, jobject java_instance, jobject java_other) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        if(!java_other) {
            return JSValueIsNull(context, value);
        }

        auto [other_ok, other_value] = extract_secondary(env, java_other);
        if(!other_ok) {
            return false;
        }

        return JSValueIsStrictEqual(context, value, other_value);
    }

    bool JavascriptValueJNI::is_instance_of_constructor(JNIEnv *env, jobject java_instance, jobject java_constructor) {
        auto [ok, context, value, lock] = extract_with_lock(env, java_instance);
        if(!ok) {
            return false;
        }

        auto constructor = reinterpret_cast<JSObjectRef>(
            env->CallLongMethod(java_constructor, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return false;
        }

        JSValueRef exception = nullptr;
        bool is_instance = JSValueIsInstanceOfConstructor(context, value, constructor, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Error while checking if value is instance of constructor", context, exception, env, lock);
            return false;
        }

        return is_instance;
    }

    jstring JavascriptValueJNI::to_json(JNIEnv *env, jobject java_instance, jshort indentation) {
        auto [ok, context, value, lock] = extract_with_lock(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        auto json_ref = JSValueCreateJSONString(context, value, indentation, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Exception while converting the value to a json string", context, exception, env, lock);
            return nullptr;
        }

        auto java_string = Util::create_jstring_from_jsstring_ref(env, json_ref);
        JSStringRelease(json_ref);

        return java_string;
    }

    jboolean JavascriptValueJNI::to_boolean(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value] = extract(env, java_instance);
        if(!ok) {
            return false;
        }

        return JSValueToBoolean(context, value);
    }

    jdouble JavascriptValueJNI::to_number(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value, lock] = extract_with_lock(env, java_instance);
        if(!ok) {
            return false;
        }

        JSValueRef exception = nullptr;
        jdouble java_double = JSValueToNumber(context, value, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Exception while converting the value to a number", context, exception, env, lock);
            return 0;
        }

        return java_double;
    }

    jstring JavascriptValueJNI::to_string_copy(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value, lock] = extract_with_lock(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        JSStringRef string_ref = JSValueToStringCopy(context, value, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Exception while converting the value to a string", context, exception, env, lock);
            return nullptr;
        }

        auto java_string = Util::create_jstring_from_jsstring_ref(env, string_ref);
        JSStringRelease(string_ref);

        return java_string;
    }

    jobject JavascriptValueJNI::to_object(JNIEnv *env, jobject java_instance) {
        auto [ok, context, value, lock] = extract_with_lock(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        JSObjectRef object = JSValueToObject(context, value, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception(
                "Exception while converting the value to an object", context, exception, env, lock);
            return nullptr;
        }

        JSValueProtect(context, object);

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(object),
            lock);
    }
} // namespace ultralight_java