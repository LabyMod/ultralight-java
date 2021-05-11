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

#include "ultralight_java/java_bridges/javascript_global_context_jni.hpp"

#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    void JavascriptGlobalContextJNI::context_unlocking(JNIEnv *env, jobject java_instance) {
        auto context = reinterpret_cast<JSGlobalContextRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return;
        }

        JSGlobalContextRelease(context);
    }

    jstring JavascriptGlobalContextJNI::get_name(JNIEnv *env, jobject java_instance) {
        auto context = reinterpret_cast<JSGlobalContextRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        auto javascript_name = JSGlobalContextCopyName(context);
        if(!javascript_name) {
            return nullptr;
        }

        auto java_name = Util::create_jstring_from_jsstring_ref(env, javascript_name);
        JSStringRelease(javascript_name);

        return java_name;
    }

    void JavascriptGlobalContextJNI::set_name(JNIEnv *env, jobject java_instance, jstring java_name) {
        auto context = reinterpret_cast<JSGlobalContextRef>(
            env->CallLongMethod(java_instance, runtime.object_with_handle.get_handle_method));
        if(env->ExceptionCheck()) {
            return;
        }

        if(!java_name) {
            JSGlobalContextSetName(context, nullptr);
            return;
        }

        auto javascript_name = Util::create_jsstring_ref_from_jstring(env, java_name);
        JSGlobalContextSetName(context, javascript_name);
    }
} // namespace ultralight_java