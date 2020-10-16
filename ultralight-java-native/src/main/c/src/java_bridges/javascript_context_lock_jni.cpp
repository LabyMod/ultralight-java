/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
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

#include "ultralight_java/java_bridges/javascript_context_lock_jni.hpp"

#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    HoldJavascriptContextLock::~HoldJavascriptContextLock() {
        switch(type) {
            case ULTRALIGHT:
                ultralight_lock.~Ref();
                break;

            case JNI:
                jni_lock = nullptr;
                break;
        }
    }
    HoldJavascriptContextLock::HoldJavascriptContextLock(ultralight::Ref<ultralight::JSContext> lock)
        : type(ULTRALIGHT), ultralight_lock(std::move(lock)) {
    }

    HoldJavascriptContextLock::HoldJavascriptContextLock(JSContextRef lock) : type(JNI), jni_lock(lock) {
    }

    JSContextRef HoldJavascriptContextLock::get_context() {
        switch(type) {
            case ULTRALIGHT:
                return ultralight_lock->ctx();

            case JNI:
                return jni_lock;
        }

        assert(!"UNREACHABLE");
        return nullptr;
    }

    jobject JavascriptContextLockJNI::create(JNIEnv *env, ultralight::Ref<ultralight::JSContext> lock) {
        auto *hold_lock = new HoldJavascriptContextLock(std::move(lock));
        return env->NewObject(
            runtime.javascript_context_lock.clazz,
            runtime.javascript_context_lock.constructor,
            reinterpret_cast<jlong>(hold_lock));
    }

    jobject JavascriptContextLockJNI::create(JNIEnv *env, JSContextRef context) {
        auto *hold_lock = new HoldJavascriptContextLock(context);
        return env->NewObject(
            runtime.javascript_context_lock.clazz,
            runtime.javascript_context_lock.constructor,
            reinterpret_cast<jlong>(hold_lock));
    }

    jobject JavascriptContextLockJNI::get_context(JNIEnv *env, jobject java_instance) {
        auto *lock = reinterpret_cast<HoldJavascriptContextLock *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        auto context = lock->get_context();
        return env->NewObject(
            runtime.javascript_context.clazz,
            runtime.javascript_context.constructor,
            reinterpret_cast<jlong>(context),
            java_instance);
    }

    void JavascriptContextLockJNI::release(JNIEnv *env, jobject java_instance) {
        auto *lock = reinterpret_cast<HoldJavascriptContextLock *>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return;
        }

        delete lock;
    }
} // namespace ultralight_java