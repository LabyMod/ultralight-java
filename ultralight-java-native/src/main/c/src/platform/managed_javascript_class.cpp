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

#include "ultralight_java/platform/managed_javascript_class.hpp"

#include <ultralight_java/ultralight_java_instance.hpp>

#include "ultralight_java/java_bridges/javascript_context_lock_jni.hpp"
#include "ultralight_java/java_bridges/proxied_java_exception.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/local_jni_reference_wrapper.hpp"
#include "ultralight_java/util/temporary_jni.hpp"
#include "ultralight_java/util/util.hpp"

#define WRAP_VALUE(env, ctx, handle, lock)                                                                             \
    ([&] {                                                                                                             \
        JSValueProtect(ctx, handle);                                                                                   \
        return LocalJNIReferenceWrapper<jobject>::construct(                                                           \
            env,                                                                                                       \
            runtime.javascript_value.clazz,                                                                            \
            runtime.javascript_value.constructor,                                                                      \
            reinterpret_cast<jlong>(handle),                                                                           \
            lock.get());                                                                                               \
    })()

#define WRAP_OBJECT(env, ctx, handle, lock)                                                                            \
    ([&] {                                                                                                             \
        JSValueProtect(ctx, handle);                                                                                   \
        return LocalJNIReferenceWrapper<jobject>::construct(                                                           \
            env,                                                                                                       \
            runtime.javascript_object.clazz,                                                                           \
            runtime.javascript_object.constructor,                                                                     \
            reinterpret_cast<jlong>(handle),                                                                           \
            lock.get());                                                                                               \
    })()

#define WRAP_CONTEXT(env, handle, lock)                                                                                \
    LocalJNIReferenceWrapper<jobject>::construct(                                                                      \
        env,                                                                                                           \
        runtime.javascript_context.clazz,                                                                              \
        runtime.javascript_context.constructor,                                                                        \
        reinterpret_cast<jlong>(handle),                                                                               \
        lock.get())

namespace ultralight_java {
    ManagedJavascriptPrivateData::ManagedJavascriptPrivateData(JNIEnv *env, jobject reference) : reference(env->NewGlobalRef(reference)), ref_count(0) {
    }

    ManagedJavascriptPrivateData::~ManagedJavascriptPrivateData() {
        assert(ref_count == 0);
    }

    jobject ManagedJavascriptPrivateData::get_inner() const {
        return reference;
    }

    void ManagedJavascriptPrivateData::ref() {
        assert(ref_count >= 0);
        ref_count++;
    }

    bool ManagedJavascriptPrivateData::deref() {
        assert(ref_count > 0);
        return --ref_count == 0;
    }

    uint64_t ManagedJavascriptPrivateData::get_ref_count() const {
        return ref_count;
    }

    void ManagedJavascriptPrivateData::swap(JNIEnv *env, jobject new_reference) {
        assert(ref_count > 0);

        if(reference) {
            env->DeleteGlobalRef(reference);
        }

        if(new_reference) {
            reference = env->NewGlobalRef(new_reference);
        } else {
            reference = nullptr;
        }
    }

    ManagedJavascriptFunctionContainer::ManagedJavascriptFunctionContainer()
        : java_initializer(nullptr),
          java_finalizer(nullptr),
          java_has_property_tester(nullptr),
          java_property_getter(nullptr),
          java_property_setter(nullptr),
          java_property_deleter(nullptr),
          java_property_name_collector(nullptr),
          java_call_as_function(nullptr),
          java_call_as_constructor(nullptr),
          java_has_instance_tester(nullptr),
          java_to_type_converter(nullptr) {
    }

    ManagedJavascriptStaticField::ManagedJavascriptStaticField() : getter(nullptr), setter(nullptr), attributes(0) {
    }

    ManagedJavascriptStaticFunction::ManagedJavascriptStaticFunction() : function(nullptr), attributes(0) {
    }

    ManagedJavascriptClassData::ManagedJavascriptClassData() = default;

    void ManagedJavascriptCallbacks::initialize(JSContextRef ctx, JSClassRef clazz, JSObjectRef object) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));
        auto *private_data = reinterpret_cast<ManagedJavascriptPrivateData *>(JSObjectGetPrivate(object));
        private_data->ref();

        if(class_data->functions.java_initializer) {
            TemporaryJNI env;
            LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
            auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
            auto java_object = WRAP_OBJECT(env, ctx, object, java_lock);
            if(env->ExceptionCheck() && java_lock) {
                env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            }
            ProxiedJavaException::throw_if_any(env);

            env->CallVoidMethod(
                class_data->functions.java_initializer,
                runtime.javascript_object_initializer.initialize_javascript_object_method,
                java_context.get(),
                java_object.get());

            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);

            ProxiedJavaException::throw_if_any(env);
        }
    }

    void ManagedJavascriptCallbacks::finalize(JSClassRef clazz, JSObjectRef object) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));
        auto *private_data = reinterpret_cast<ManagedJavascriptPrivateData *>(JSObjectGetPrivate(object));

        TemporaryJNI env;
        if(class_data->functions.java_finalizer) {
            auto java_object = LocalJNIReferenceWrapper<jobject>::construct(
                env,
                runtime.javascript_object.clazz,
                runtime.javascript_object.constructor,
                reinterpret_cast<jlong>(object),
                nullptr);

            env->CallVoidMethod(
                class_data->functions.java_finalizer,
                runtime.javascript_object_finalizer.finalize_javascript_object,
                java_object.get());
        }


        if(private_data->deref()) {
            env->DeleteGlobalRef(private_data->get_inner());
            delete private_data;
        }

        ProxiedJavaException::throw_if_any(env);
    }

    bool ManagedJavascriptCallbacks::has_property(
        JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSStringRef property_name) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_object = WRAP_OBJECT(env, ctx, object, java_lock);
        if(env->ExceptionCheck() && java_lock) {
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
        }

        ProxiedJavaException::throw_if_any(env);

        LocalJNIReferenceWrapper<jstring>
            java_property_name(env, Util::create_jstring_from_jsstring_ref(env, property_name));

        jboolean ret = env->CallBooleanMethod(
            class_data->functions.java_has_property_tester,
            runtime.javascript_object_has_property_tester.has_javascript_property_method,
            java_context.get(),
            java_object.get(),
            java_property_name.get());

        env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);

        ProxiedJavaException::throw_if_any(env);

        return ret;
    }

    JSValueRef ManagedJavascriptCallbacks::get_property(
        JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSStringRef property_name, JSValueRef *exception) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_object = WRAP_OBJECT(env, ctx, object, java_lock);
        LocalJNIReferenceWrapper<jstring>
            java_property_name(env, Util::create_jstring_from_jsstring_ref(env, property_name));

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            if(java_lock) {
                env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            }
            return nullptr;
        }

        LocalJNIReferenceWrapper<jobject>
            ret(env,
                env->CallObjectMethod(
                    class_data->functions.java_property_getter,
                    runtime.javascript_object_property_getter.get_javascript_property_method,
                    java_context.get(),
                    java_object.get(),
                    java_property_name.get()));
        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return nullptr;
        }

        if(!ret) {
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return nullptr;
        }

        auto value = reinterpret_cast<JSValueRef>(
            env->CallLongMethod(ret, runtime.object_with_handle.get_handle_method));
        JSValueProtect(ctx, value);

        env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);

        return value;
    }

    JSValueRef ManagedJavascriptCallbacks::get_static_property(
        JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSStringRef property_name, JSValueRef *exception) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_object = WRAP_OBJECT(env, ctx, object, java_lock);
        LocalJNIReferenceWrapper<jstring>
            java_property_name(env, Util::create_jstring_from_jsstring_ref(env, property_name));

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            if(java_lock) {
                env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            }
            return nullptr;
        }

        auto getter_index = Util::create_utf8_from_jsstring_ref(property_name);
        LocalJNIReferenceWrapper<jobject> ret(env);

        if(auto it = class_data->static_fields.find(getter_index); it != class_data->static_fields.end()) {
            ret = LocalJNIReferenceWrapper<jobject>(
                env,
                env->CallObjectMethod(
                    it->second.getter,
                    runtime.javascript_object_property_getter.get_javascript_property_method,
                    java_context.get(),
                    java_object.get(),
                    java_property_name.get()));
            
        } else {
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return nullptr;
        }

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return nullptr;
        }

        if(!ret) {
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return nullptr;
        }

        auto value = reinterpret_cast<JSValueRef>(
            env->CallLongMethod(ret, runtime.object_with_handle.get_handle_method));
        JSValueProtect(ctx, value);

        env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);

        return value;
    }

    bool ManagedJavascriptCallbacks::set_property(
        JSContextRef ctx,
        JSClassRef clazz,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef value,
        JSValueRef *exception) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_object = WRAP_OBJECT(env, ctx, object, java_lock);
        auto java_value = WRAP_VALUE(env, ctx, value, java_lock);
        LocalJNIReferenceWrapper<jstring>
            java_property_name(env, Util::create_jstring_from_jsstring_ref(env, property_name));

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();

            if(java_lock) {
                env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            }
            return false;
        }

        jboolean ret = env->CallBooleanMethod(
            class_data->functions.java_property_setter,
            runtime.javascript_object_property_setter.set_javascript_property_method,
            java_context.get(),
            java_object.get(),
            java_property_name.get(),
            java_value.get());

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return false;
        }

        return ret;
    }

    bool ManagedJavascriptCallbacks::set_static_property(
        JSContextRef ctx,
        JSClassRef clazz,
        JSObjectRef object,
        JSStringRef property_name,
        JSValueRef value,
        JSValueRef *exception) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_object = WRAP_OBJECT(env, ctx, object, java_lock);
        auto java_value = WRAP_VALUE(env, ctx, value, java_lock);
        LocalJNIReferenceWrapper<jstring>
            java_property_name(env, Util::create_jstring_from_jsstring_ref(env, property_name));

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();

            if(java_lock) {
                env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            }
            return false;
        }

        auto setter_index = Util::create_utf8_from_jsstring_ref(property_name);

        jboolean ret = false;
        if(auto it = class_data->static_fields.find(setter_index); it != class_data->static_fields.end()) {
            ret = env->CallBooleanMethod(
                it->second.setter,
                runtime.javascript_object_property_setter.set_javascript_property_method,
                java_context.get(),
                java_object.get(),
                java_property_name.get(),
                java_value.get());
        } else {
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return false;
        }

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return false;
        }

        return ret;
    }

    bool ManagedJavascriptCallbacks::delete_property(
        JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSStringRef property_name, JSValueRef *exception) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_object = WRAP_OBJECT(env, ctx, object, java_lock);
        LocalJNIReferenceWrapper<jstring>
            java_property_name(env, Util::create_jstring_from_jsstring_ref(env, property_name));

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return false;
        }

        jboolean ret = env->CallBooleanMethod(
            class_data->functions.java_property_deleter,
            runtime.javascript_object_property_deleter.delete_javascript_property_method,
            java_context.get(),
            java_object.get(),
            java_property_name.get());

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return false;
        }

        return ret;
    }
    void ManagedJavascriptCallbacks::get_property_names(
        JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSPropertyNameAccumulatorRef property_names) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_object = WRAP_OBJECT(env, ctx, object, java_lock);
        if(env->ExceptionCheck() && java_lock) {
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
        }

        ProxiedJavaException::throw_if_any(env);

        LocalJNIReferenceWrapper<jobjectArray> java_property_names(
            env,
            reinterpret_cast<jobjectArray>(env->CallObjectMethod(
                class_data->functions.java_property_name_collector,
                runtime.javascript_object_property_names_collector.collect_javascript_property_names_method,
                java_context.get(),
                java_object.get())));

        ProxiedJavaException::throw_if_any(env);

        if(!java_property_names) {
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            env->ThrowNew(runtime.null_pointer_exception.clazz, "collectJavascriptPropertyNames returned null");
            ProxiedJavaException::throw_if_any(env);
        }

        size_t array_length = env->GetArrayLength(java_property_names);

        for(size_t i = 0; i < array_length; i++) {
            auto java_name = reinterpret_cast<jstring>(env->GetObjectArrayElement(java_property_names, i));
            if(!java_name) {
                continue;
            }

            JSStringRef javascript_name = Util::create_jsstring_ref_from_jstring(env, java_name);
            JSPropertyNameAccumulatorAddName(property_names, javascript_name);
            JSStringRelease(javascript_name);

            env->DeleteLocalRef(java_name);
        }
    }

    JSValueRef ManagedJavascriptCallbacks::call_as_function(
        JSContextRef ctx,
        JSClassRef clazz,
        JSStringRef /*function_name*/,
        JSObjectRef function,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef *arguments,
        JSValueRef *exception) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        env->PushLocalFrame(argument_count + 8);

        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_function = WRAP_OBJECT(env, ctx, function, java_lock);
        auto java_this_object = WRAP_OBJECT(env, ctx, this_object, java_lock);

        LocalJNIReferenceWrapper<jobjectArray>
            java_arguments(env, env->NewObjectArray(argument_count, runtime.javascript_value.clazz, nullptr));

        for(size_t i = 0; i < argument_count; i++) {
            JSValueRef argument = arguments[i];
            auto java_argument = WRAP_VALUE(env, ctx, argument, java_lock);
            env->SetObjectArrayElement(java_arguments.get(), i, java_argument.get());
        }

        if(env->ExceptionCheck()) {
            if(java_lock) {
                env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            }

            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->PopLocalFrame(nullptr);
            return nullptr;
        }

        LocalJNIReferenceWrapper<jobject>
            ret(env,
                env->CallObjectMethod(
                    class_data->functions.java_call_as_function,
                    runtime.javascript_object_function.call_as_javascript_function,
                    java_context.get(),
                    java_function.get(),
                    java_this_object.get(),
                    java_arguments.get()));

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            env->PopLocalFrame(nullptr);
            return nullptr;
        } else if(!ret) {
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            *exception = Util::create_jserror(ctx, "callAsJavascriptFunction returned null");
            env->PopLocalFrame(nullptr);
            return nullptr;
        }

        auto value = reinterpret_cast<JSValueRef>(
            env->CallLongMethod(ret, runtime.object_with_handle.get_handle_method));
        env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
        env->PopLocalFrame(nullptr);
        JSValueProtect(ctx, value);

        return value;
    }

    JSValueRef ManagedJavascriptCallbacks::call_static_function(
        JSContextRef ctx,
        JSClassRef clazz,
        JSStringRef function_name,
        JSObjectRef function,
        JSObjectRef this_object,
        size_t argument_count,
        const JSValueRef *arguments,
        JSValueRef *exception) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        env->PushLocalFrame(argument_count + 8);

        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_function = WRAP_OBJECT(env, ctx, function, java_lock);
        auto java_this_object = WRAP_OBJECT(env, ctx, this_object, java_lock);

        LocalJNIReferenceWrapper<jobjectArray>
            java_arguments(env, env->NewObjectArray(argument_count, runtime.javascript_value.clazz, nullptr));

        for(size_t i = 0; i < argument_count; i++) {
            JSValueRef argument = arguments[i];
            auto java_argument = WRAP_VALUE(env, ctx, argument, java_lock);
            env->SetObjectArrayElement(java_arguments.get(), i, java_argument.get());
        }

        if(env->ExceptionCheck()) {
            if(java_lock) {
                env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            }

            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->PopLocalFrame(nullptr);
            return nullptr;
        }

        auto function_index = Util::create_utf8_from_jsstring_ref(function_name);
        LocalJNIReferenceWrapper<jobject> ret(env);
        if(auto it = class_data->static_functions.find(function_index); it != class_data->static_functions.end()) {
            ret = LocalJNIReferenceWrapper<jobject>(
                env,
                env->CallObjectMethod(
                    it->second.function,
                    runtime.javascript_object_function.call_as_javascript_function,
                    java_context.get(),
                    java_function.get(),
                    java_this_object.get(),
                    java_arguments.get()));
        } else {
            env->ThrowNew(
                runtime.illegal_argument_exception.clazz,
                ("Tried to get call non existent static function " + function_index).c_str());
        }

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            env->PopLocalFrame(nullptr);
            return nullptr;
        } else if(!ret) {
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            *exception = Util::create_jserror(ctx, "callAsJavascriptFunction returned null");
            env->PopLocalFrame(nullptr);
            return nullptr;
        }

        auto value = reinterpret_cast<JSValueRef>(
            env->CallLongMethod(ret, runtime.object_with_handle.get_handle_method));
        env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
        env->PopLocalFrame(nullptr);
        JSValueProtect(ctx, value);

        return value;
    }

    JSObjectRef ManagedJavascriptCallbacks::call_as_constructor(
        JSContextRef ctx,
        JSClassRef clazz,
        JSObjectRef constructor,
        size_t argument_count,
        const JSValueRef *arguments,
        JSValueRef *exception) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        env->PushLocalFrame(argument_count + 8);

        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_constructor = WRAP_OBJECT(env, ctx, constructor, java_lock);

        LocalJNIReferenceWrapper<jobjectArray>
            java_arguments(env, env->NewObjectArray(argument_count, runtime.javascript_value.clazz, nullptr));

        for(size_t i = 0; i < argument_count; i++) {
            JSValueRef argument = arguments[i];
            auto java_argument = WRAP_VALUE(env, ctx, argument, java_lock);
            env->SetObjectArrayElement(java_arguments.get(), i, java_argument.get());
        }

        if(env->ExceptionCheck()) {
            if(java_lock) {
                env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            }

            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->PopLocalFrame(nullptr);
            return nullptr;
        }

        LocalJNIReferenceWrapper<jobject>
            ret(env,
                env->CallObjectMethod(
                    class_data->functions.java_call_as_constructor,
                    runtime.javascript_object_constructor.call_as_javascript_constructor_method,
                    java_context.get(),
                    java_constructor.get(),
                    java_arguments.get()));

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            env->PopLocalFrame(nullptr);
            return nullptr;
        } else if(!ret) {
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            *exception = Util::create_jserror(ctx, "callAsJavascriptConstructor returned null");
            env->PopLocalFrame(nullptr);
            return nullptr;
        }

        auto value = reinterpret_cast<JSObjectRef>(
            env->CallLongMethod(ret, runtime.object_with_handle.get_handle_method));
        env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
        env->PopLocalFrame(nullptr);
        JSValueProtect(ctx, value);

        return value;
    }

    bool ManagedJavascriptCallbacks::has_instance(
        JSContextRef ctx,
        JSClassRef clazz,
        JSObjectRef constructor,
        JSValueRef possible_instance,
        JSValueRef *exception) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_constructor = WRAP_OBJECT(env, ctx, constructor, java_lock);
        auto java_possible_instance = WRAP_OBJECT(env, ctx, possible_instance, java_lock);

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return false;
        }

        jboolean ret = env->CallBooleanMethod(
            class_data->functions.java_has_instance_tester,
            runtime.javascript_object_has_instance_tester.has_javascript_instance_method,
            java_context.get(),
            java_constructor.get(),
            java_possible_instance.get());

        env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return false;
        }

        return ret;
    }

    JSValueRef ManagedJavascriptCallbacks::convert_to_type(
        JSContextRef ctx, JSClassRef clazz, JSObjectRef object, JSType type, JSValueRef *exception) {
        auto *class_data = reinterpret_cast<ManagedJavascriptClassData *>(JSClassGetPrivate(clazz));

        TemporaryJNI env;
        LocalJNIReferenceWrapper<jobject> java_lock(env, JavascriptContextLockJNI::create(env, ctx));
        auto java_context = WRAP_CONTEXT(env, ctx, java_lock);
        auto java_object = WRAP_OBJECT(env, ctx, object, java_lock);
        LocalJNIReferenceWrapper<jobject> java_type(env, runtime.javascript_type.constants.to_java(env, type));

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return nullptr;
        }

        LocalJNIReferenceWrapper<jobject>
            ret(env,
                env->CallObjectMethod(
                    class_data->functions.java_to_type_converter,
                    runtime.javascript_object_to_type_converter.convert_to_javascript_type_method,
                    java_context.get(),
                    java_object.get(),
                    java_type.get()));

        if(env->ExceptionCheck()) {
            *exception = Util::create_jssvalue_from_jthrowable(env, env->ExceptionOccurred(), ctx);
            env->ExceptionClear();
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            return nullptr;
        } else if(!ret) {
            env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
            *exception = Util::create_jserror(ctx, "convertToJavascriptType returned null");
            return nullptr;
        }

        auto value = reinterpret_cast<JSObjectRef>(
            env->CallLongMethod(ret, runtime.object_with_handle.get_handle_method));
        env->CallVoidMethod(java_lock, runtime.javascript_context_lock.unlock_method);
        JSValueProtect(ctx, value);

        return value;
    }
} // namespace ultralight_java
