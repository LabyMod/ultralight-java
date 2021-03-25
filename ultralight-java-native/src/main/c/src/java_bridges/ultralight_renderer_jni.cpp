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

#include "ultralight_java/java_bridges/ultralight_renderer_jni.hpp"

#include <Ultralight/Ultralight.h>

#include "ultralight_java/java_bridges/proxied_java_exception.hpp"
#include "ultralight_java/java_bridges/ultralight_ref_ptr_jni.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    jobject UltralightRendererJNI::create(JNIEnv *env, jclass caller_class) {
        // Create the renderer instance
        auto renderer = ultralight::Renderer::Create();

        // Package the instance into a java object
        jobject
            pointer = UltralightRefPtrJNI::create(env, ultralight::RefPtr<ultralight::Renderer>(std::move(renderer)));

        if(env->ExceptionCheck()) {
            // An exception occurred in java, don't try to construct a new object
            return nullptr;
        }

        // Create the instance of the ultralight renderer
        return env->NewObject(runtime.ultralight_renderer.clazz, runtime.ultralight_renderer.constructor, pointer);
    }

    jobject UltralightRendererJNI::create_view(
        JNIEnv *env, jobject instance, jlong width, jlong height, jobject java_config) {
        auto renderer = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Renderer>(env, instance);
        if(env->ExceptionCheck()) {
            return nullptr;
        }

        auto &config_type = runtime.ultralight_view_config;

        ultralight::ViewConfig config;
        ASSIGN_CONFIG_RET(Boolean, is_accelerated, nullptr);
        ASSIGN_CONFIG_RET(Boolean, is_transparent, nullptr);
        ASSIGN_CONFIG_RET(Double, initial_device_scale, nullptr);
        ASSIGN_CONFIG_RET(Boolean, initial_focus, nullptr);
        ASSIGN_CONFIG_RET(Boolean, enable_images, nullptr);
        ASSIGN_CONFIG_RET(Boolean, enable_javascript, nullptr);
        ASSIGN_CONFIG_STRING_RET(font_family_standard, "fontFamilyStandard can't be null", nullptr);
        ASSIGN_CONFIG_STRING_RET(font_family_fixed, "fontFamilyFixed can't be null", nullptr);
        ASSIGN_CONFIG_STRING_RET(font_family_serif, "fontFamilySerif can't be null", nullptr);
        ASSIGN_CONFIG_STRING_RET(font_family_sans_serif, "fontFamilySansSerif can't be null", nullptr);
        ASSIGN_CONFIG_STRING_RET(user_agent, "userAgent can't be null", nullptr);

        // Create the view RefPtr
        auto view = renderer->CreateView(width, height, config, nullptr, force_cpu_renderer);

        // Convert the RefPtr to a java object
        jobject pointer = UltralightRefPtrJNI::create(env, ultralight::RefPtr<ultralight::View>(std::move(view)));

        // Create the instance of the ultralight view
        return env->NewObject(runtime.ultralight_view.clazz, runtime.ultralight_view.constructor, pointer);
    }

    void UltralightRendererJNI::update(JNIEnv *env, jobject instance) {
        auto renderer = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Renderer>(env, instance);
        if(env->ExceptionCheck()) {
            return;
        }

        try {
            renderer->Update();
        } catch(ProxiedJavaException &ex) {
            ex.throw_to_java(env);
        }
    }

    void UltralightRendererJNI::render(JNIEnv *env, jobject instance) {
        auto renderer = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Renderer>(env, instance);
        if(env->ExceptionCheck()) {
            return;
        }

        try {
            renderer->Render();
        } catch(ProxiedJavaException &ex) {
            ex.throw_to_java(env);
        }
    }

    void UltralightRendererJNI::purgeMemory(JNIEnv *env, jobject instance) {
        auto renderer = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Renderer>(env, instance);
        if(env->ExceptionCheck()) {
            return;
        }

        renderer->PurgeMemory();
    }

    void UltralightRendererJNI::logMemoryUsage(JNIEnv *env, jobject instance) {
        auto renderer = UltralightRefPtrJNI::unwrap_ref_ptr<ultralight::Renderer>(env, instance);
        if(env->ExceptionCheck()) {
            return;
        }

        try {
            renderer->LogMemoryUsage();
        } catch(ProxiedJavaException &ex) {
            ex.throw_to_java(env);
        }
    }
} // namespace ultralight_java
