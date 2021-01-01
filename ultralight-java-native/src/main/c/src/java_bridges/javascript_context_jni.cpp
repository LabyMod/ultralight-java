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

#include "ultralight_java/java_bridges/javascript_context_jni.hpp"

#include <JavaScriptCore/JavaScript.h>
#include <ultralight_java/ultralight_java_instance.hpp>

#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    static std::tuple<bool, JSContextRef, jobject> extract(JNIEnv *env, jobject java_instance) {
        auto context = reinterpret_cast<JSContextRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return {false, nullptr, nullptr};
        }

        jobject lock = env->CallObjectMethod(java_instance, runtime.javascript_locked_object.get_lock_method);
        if(env->ExceptionCheck()) {
            return {false, nullptr, nullptr};
        }

        return {true, context, lock};
    }

    jobject JavascriptContextJNI::get_global_context(JNIEnv *env, jobject java_instance) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        auto global_context = JSGlobalContextRetain(JSContextGetGlobalContext(context));

        return env->NewObject(
            runtime.javascript_global_context.clazz,
            runtime.javascript_global_context.constructor,
            reinterpret_cast<jlong>(global_context),
            lock);
    }

    jobject JavascriptContextJNI::get_global_object(JNIEnv *env, jobject java_instance) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSObjectRef global_object = JSContextGetGlobalObject(context);

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(global_object),
            lock);
    }

    jobject JavascriptContextJNI::make_undefined(JNIEnv *env, jobject java_instance) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSValueRef value = JSValueMakeUndefined(context);

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(value),
            lock);
    }

    jobject JavascriptContextJNI::make_null(JNIEnv *env, jobject java_instance) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSValueRef value = JSValueMakeNull(context);

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(value),
            lock);
    }

    jobject JavascriptContextJNI::make_boolean(JNIEnv *env, jobject java_instance, jboolean value) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSValueRef new_value = JSValueMakeBoolean(context, value);

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(new_value),
            lock);
    }

    jobject JavascriptContextJNI::make_number(JNIEnv *env, jobject java_instance, jdouble value) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSValueRef new_value = JSValueMakeNumber(context, value);

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(new_value),
            lock);
    }

    jobject JavascriptContextJNI::make_string(JNIEnv *env, jobject java_instance, jstring java_value) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSStringRef value = Util::create_jsstring_ref_from_jstring(env, java_value);
        JSValueRef new_value = JSValueMakeString(context, value);
        JSStringRelease(value);

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(new_value),
            lock);
    }

    jobject JavascriptContextJNI::make_symbol(JNIEnv *env, jobject java_instance, jstring java_value) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSStringRef value = Util::create_jsstring_ref_from_jstring(env, java_value);
        JSValueRef new_value = JSValueMakeSymbol(context, value);
        JSStringRelease(value);

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(new_value),
            lock);
    }

    jobject JavascriptContextJNI::make_from_json(JNIEnv *env, jobject java_instance, jstring java_data) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        JSStringRef value = Util::create_jsstring_ref_from_jstring(env, java_data);
        JSValueRef new_value = JSValueMakeFromJSONString(context, value);
        JSStringRelease(value);

        if(!new_value) {
            return nullptr;
        }

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(new_value),
            lock);
    }

    jobject JavascriptContextJNI::make_object(
        JNIEnv *env, jobject java_instance, jobject java_javascript_class, jobject private_data) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        auto clazz = java_javascript_class ?
                         reinterpret_cast<JSClassRef>(
                             env->CallLongMethod(java_javascript_class, runtime.object_with_handle.get_handle_method)) :
                         nullptr;
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        auto object = JSObjectMake(context, clazz, env->NewGlobalRef(private_data));

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(object),
            lock);
    }

    jobject JavascriptContextJNI::make_array(JNIEnv *env, jobject java_instance, jobjectArray java_arguments) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        size_t argument_count = env->GetArrayLength(java_arguments);
        auto arguments = Util::translate_bridged_arguments(env, java_arguments);
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        auto array = JSObjectMakeArray(context, argument_count, arguments, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception("Error while creating array", context, exception, env, lock);
            return nullptr;
        }

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(array),
            lock);
    }

    jobject JavascriptContextJNI::make_date(JNIEnv *env, jobject java_instance, jobjectArray java_arguments) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        size_t argument_count = env->GetArrayLength(java_arguments);
        auto arguments = Util::translate_bridged_arguments(env, java_arguments);
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        auto date = JSObjectMakeDate(context, argument_count, arguments, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception("Error while creating Date", context, exception, env, lock);
            return nullptr;
        }

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(date),
            lock);
    }

    jobject JavascriptContextJNI::make_error(JNIEnv *env, jobject java_instance, jobjectArray java_arguments) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        size_t argument_count = env->GetArrayLength(java_arguments);
        auto arguments = Util::translate_bridged_arguments(env, java_arguments);
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        auto error = JSObjectMakeError(context, argument_count, arguments, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception("Error while creating Error", context, exception, env, lock);
            return nullptr;
        }

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(error),
            lock);
    }

    jobject JavascriptContextJNI::make_reg_exp(JNIEnv *env, jobject java_instance, jobjectArray java_arguments) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        }

        size_t argument_count = env->GetArrayLength(java_arguments);
        auto arguments = Util::translate_bridged_arguments(env, java_arguments);
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        JSValueRef exception = nullptr;
        auto reg_exp = JSObjectMakeRegExp(context, argument_count, arguments, &exception);
        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception("Error while creating RegExp", context, exception, env, lock);
            return nullptr;
        }

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(reg_exp),
            lock);
    }

    jobject JavascriptContextJNI::make_function(
        JNIEnv *env,
        jobject java_instance,
        jstring java_name,
        jobjectArray java_parameter_names,
        jstring java_body,
        jstring java_source_url,
        jint starting_line_number) {
        auto [ok, context, lock] = extract(env, java_instance);
        if(!ok) {
            return nullptr;
        } else if(!java_body) {
            env->ThrowNew(runtime.null_pointer_exception.clazz, "body can't be null");
            return nullptr;
        }

        auto javascript_name = java_name ? Util::create_jsstring_ref_from_jstring(env, java_name) : nullptr;
        auto javascript_parameter_names_count = java_parameter_names ? env->GetArrayLength(java_parameter_names) : 0;
        auto javascript_parameter_names = javascript_parameter_names_count ?
                                              new JSStringRef[javascript_parameter_names_count] :
                                              nullptr;

        for(size_t i = 0; i < javascript_parameter_names_count; i++) {
            auto java_parameter_name = reinterpret_cast<jstring>(env->GetObjectArrayElement(java_parameter_names, i));
            javascript_parameter_names[i] = Util::create_jsstring_ref_from_jstring(env, java_parameter_name);
            env->DeleteLocalRef(java_parameter_name);
        }

        auto javascript_body = Util::create_jsstring_ref_from_jstring(env, java_body);
        auto javascript_source_url = java_source_url ? Util::create_jsstring_ref_from_jstring(env, java_source_url) :
                                                       nullptr;

        JSValueRef exception = nullptr;
        auto function = JSObjectMakeFunction(
            context,
            javascript_name,
            javascript_parameter_names_count,
            javascript_parameter_names,
            javascript_body,
            javascript_source_url,
            starting_line_number,
            &exception);

        if(javascript_source_url) {
            JSStringRelease(javascript_source_url);
        }

        if(javascript_body) {
            JSStringRelease(javascript_body);
        }

        for(size_t i = 0; i < javascript_parameter_names_count; i++) {
            JSStringRelease(javascript_parameter_names[i]);
        }
        delete[] javascript_parameter_names;

        if(javascript_name) {
            JSStringRelease(javascript_name);
        }

        if(exception) {
            Util::throw_jssvalue_ref_as_java_exception("Error while creating Function", context, exception, env, lock);
            return nullptr;
        }

        return env->NewObject(
            runtime.javascript_object.clazz,
            runtime.javascript_object.constructor,
            reinterpret_cast<jlong>(function),
            lock);
    }
} // namespace ultralight_java