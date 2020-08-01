#include "ultralight_java/java_bridges/javascript_value_jni.hpp"

#include <JavaScriptCore/JavaScript.h>

#include "ultralight_java/java_bridges/javascript_context_lock_jni.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    static std::tuple<bool, JSContextRef, JSValueRef> extract(JNIEnv *env, jobject java_instance) {
        auto lock = reinterpret_cast<HoldJavascriptContextLock *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
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

        auto [other_ok, _, other_value] = extract(env, java_other);
        if(!other_ok) {
            return false;
        }

        return JSValueIsStrictEqual(context, value, other_value);
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
} // namespace ultralight_java