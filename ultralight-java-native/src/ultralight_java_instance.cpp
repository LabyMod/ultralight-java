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

#include "ultralight_java/ultralight_java_instance.hpp"

#include "ultralight_java/java_bridges/bridged_logger.hpp"
#include "ultralight_java/java_bridges/ultralight_view_jni.hpp"
#include "ultralight_java/ultralight_initializer.hpp"

/**
 * Called by the JavaVM when the library is being loaded.
 */
extern "C" [[maybe_unused]] JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *) {
    return ultralight_java::setup(vm);
}

/**
 * Called by the JVM when the library is being unloaded.
 */
extern "C" [[maybe_unused]] JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *) {
    ultralight_java::destruct(vm);
}

namespace ultralight_java {
    UltralightJavaRuntime runtime;

    int setup(JavaVM *vm) {
        init_runtime_struct();

        // Initialize the runtime struct
        runtime.vm = vm;

        // Retrieve a JNI environment
        JNIEnv *env;
        int error = vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_8);

        if(error != JNI_OK) {
            // Retrieving the environment failed, bail out
            return error;
        }

        // Retrieve information about the String class
        runtime.string.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("java/lang/String")));

        // Retrieve information about the ObjectWithHandle interface
        runtime.object_with_handle.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/ffi/ObjectWithHandle")));
        runtime.object_with_handle
            .get_handle_method = env->GetMethodID(runtime.object_with_handle.clazz, "getHandle", "()J");

        // Retrieve information about the UltralightPlatform class
        runtime.ultralight_platform.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/UltralightPlatform")));
        runtime.ultralight_platform.constructor = env->GetMethodID(runtime.ultralight_platform.clazz, "<init>", "(J)V");

        // Register the native methods for the UltralightPlatform class
        env->RegisterNatives(
            runtime.ultralight_platform.clazz,
            runtime.ultralight_platform.native_methods.data(),
            runtime.ultralight_platform.native_methods.size());

        // Retrieve information about the UltralightMatrix class
        auto ultralight_matrix_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/math/UltralightMatrix")));
        runtime.ultralight_matrix.clazz = ultralight_matrix_class;
        runtime.ultralight_matrix.constructor = env->GetMethodID(runtime.ultralight_matrix.clazz, "<init>", "(J)V");

        auto &ultralight_matrix = runtime.ultralight_matrix;

        // Register native methods for the UltralightMatrix class
        env->RegisterNatives(
            runtime.ultralight_matrix.clazz,
            runtime.ultralight_matrix.native_methods.data(),
            runtime.ultralight_matrix.native_methods.size());

        // Retrieve information about the UltralightMatrix4x4 class
        auto ultralight_matrix4x4_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/math/UltralightMatrix4x4")));
        runtime.ultralight_matrix4x4.clazz = ultralight_matrix4x4_class;
        runtime.ultralight_matrix4x4
            .constructor = env->GetMethodID(runtime.ultralight_matrix4x4.clazz, "<init>", "(J)V");

        auto &ultralight_matrix4x4 = runtime.ultralight_matrix4x4;

        // Register native methods for the UltralightMatrix4x4 class
        env->RegisterNatives(
            runtime.ultralight_matrix4x4.clazz,
            runtime.ultralight_matrix4x4.native_methods.data(),
            runtime.ultralight_matrix4x4.native_methods.size());

        // Retrieve information about the UltralightConfig class
        auto ultralight_config_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/config/UltralightConfig")));
        runtime.ultralight_config.clazz = ultralight_config_class;

        auto &ultralight_config = runtime.ultralight_config;
        ultralight_config
            .resource_path_field = env->GetFieldID(ultralight_config_class, "resourcePath", "Ljava/lang/String;");
        ultralight_config
            .cache_path_field = env->GetFieldID(ultralight_config_class, "cachePath", "Ljava/lang/String;");
        ultralight_config.face_winding_field =
            env->GetFieldID(ultralight_config_class, "faceWinding", "Lcom/labymedia/ultralight/config/FaceWinding;");
        ultralight_config.font_hinting_field =
            env->GetFieldID(ultralight_config_class, "fontHinting", "Lcom/labymedia/ultralight/config/FontHinting;");
        ultralight_config.font_gamma_field = env->GetFieldID(ultralight_config_class, "fontGamma", "D");
        ultralight_config
            .user_stylesheet_field = env->GetFieldID(ultralight_config_class, "userStylesheet", "Ljava/lang/String;");
        ultralight_config.force_repaint_field = env->GetFieldID(ultralight_config_class, "forceRepaint", "Z");
        ultralight_config
            .animation_timer_delay_field = env->GetFieldID(ultralight_config_class, "animationTimerDelay", "D");
        ultralight_config.scroll_timer_delay_field = env->GetFieldID(ultralight_config_class, "scrollTimerDelay", "D");
        ultralight_config.recycle_delay_field = env->GetFieldID(ultralight_config_class, "recycleDelay", "D");
        ultralight_config.memory_cache_size_field = env->GetFieldID(ultralight_config_class, "memoryCacheSize", "J");
        ultralight_config.page_cache_size_field = env->GetFieldID(ultralight_config_class, "pageCacheSize", "J");
        ultralight_config.override_ram_size_field = env->GetFieldID(ultralight_config_class, "overrideRamSize", "J");
        ultralight_config.min_large_heap_size_field = env->GetFieldID(ultralight_config_class, "minLargeHeapSize", "J");
        ultralight_config.min_small_heap_size_field = env->GetFieldID(ultralight_config_class, "minSmallHeapSize", "J");

        // Retrieve information about the UltralightViewConfig class
        auto ultralight_view_config_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/config/UltralightViewConfig")));
        runtime.ultralight_view_config.clazz = ultralight_view_config_class;

        runtime.ultralight_view_config
            .is_accelerated_field = env->GetFieldID(ultralight_view_config_class, "isAccelerated", "Z");
        runtime.ultralight_view_config
            .is_transparent_field = env->GetFieldID(ultralight_view_config_class, "isTransparent", "Z");
        runtime.ultralight_view_config
            .initial_device_scale_field = env->GetFieldID(ultralight_view_config_class, "initialDeviceScale", "D");
        runtime.ultralight_view_config
            .initial_focus_field = env->GetFieldID(ultralight_view_config_class, "initialFocus", "Z");
        runtime.ultralight_view_config
            .enable_images_field = env->GetFieldID(ultralight_view_config_class, "enableImages", "Z");
        runtime.ultralight_view_config
            .enable_javascript_field = env->GetFieldID(ultralight_view_config_class, "enableJavascript", "Z");
        runtime.ultralight_view_config.font_family_standard_field =
            env->GetFieldID(ultralight_view_config_class, "fontFamilyStandard", "Ljava/lang/String;");
        runtime.ultralight_view_config.font_family_fixed_field =
            env->GetFieldID(ultralight_view_config_class, "fontFamilyFixed", "Ljava/lang/String;");
        runtime.ultralight_view_config.font_family_serif_field =
            env->GetFieldID(ultralight_view_config_class, "fontFamilySerif", "Ljava/lang/String;");
        runtime.ultralight_view_config.font_family_sans_serif_field =
            env->GetFieldID(ultralight_view_config_class, "fontFamilySansSerif", "Ljava/lang/String;");
        runtime.ultralight_view_config
            .user_agent_field = env->GetFieldID(ultralight_view_config_class, "userAgent", "Ljava/lang/String;");

        // Retrieve information about the FaceWinding enum
        runtime.face_winding.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/config/FaceWinding")));
        if(!runtime.face_winding.constants.init(env, "com/labymedia/ultralight/config/FaceWinding")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the FontHinting enum
        runtime.font_hinting.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/config/FontHinting")));
        if(!runtime.font_hinting.constants.init(env, "com/labymedia/ultralight/config/FontHinting")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightLogger interface
        runtime.logger.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/logging/UltralightLogger")));
        runtime.logger.log_message_method = env->GetMethodID(
            runtime.logger.clazz,
            "logMessage",
            "(Lcom/labymedia/ultralight/plugin/logging/UltralightLogLevel;Ljava/lang/String;)V");

        // Retrieve information about the UltralightLogLevel enum
        runtime.log_level.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/logging/UltralightLogLevel")));
        if(!runtime.log_level.constants.init(env, "com/labymedia/ultralight/plugin/logging/UltralightLogLevel")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the RefPtr class
        runtime.ref_ptr.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/ffi/RefPtr")));
        runtime.ref_ptr.constructor = env->GetMethodID(runtime.ref_ptr.clazz, "<init>", "(J)V");

        // Register the native methods for the RefPtr class
        env->RegisterNatives(
            runtime.ref_ptr.clazz, runtime.ref_ptr.native_methods.data(), runtime.ref_ptr.native_methods.size());

        // Retrieve information about the UltralightRenderer class
        runtime.ultralight_renderer.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/UltralightRenderer")));
        runtime.ultralight_renderer.constructor =
            env->GetMethodID(runtime.ultralight_renderer.clazz, "<init>", "(Lcom/labymedia/ultralight/ffi/RefPtr;)V");

        // Register the native methods for the UltralightRenderer class
        env->RegisterNatives(
            runtime.ultralight_renderer.clazz,
            runtime.ultralight_renderer.native_methods.data(),
            runtime.ultralight_renderer.native_methods.size());

        // Retrieve information about the UltralightView class
        runtime.ultralight_view.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/UltralightView")));
        runtime.ultralight_view.constructor =
            env->GetMethodID(runtime.ultralight_view.clazz, "<init>", "(Lcom/labymedia/ultralight/ffi/RefPtr;)V");

        // Register the native methods for the UltralightView class
        env->RegisterNatives(
            runtime.ultralight_view.clazz,
            runtime.ultralight_view.native_methods.data(),
            runtime.ultralight_view.native_methods.size());

        // Retrieve information about the IntRect class
        runtime.int_rect.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/math/IntRect")));
        runtime.int_rect.bounds_constructor = env->GetMethodID(runtime.int_rect.clazz, "<init>", "(IIII)V");
        runtime.int_rect.left_field = env->GetFieldID(runtime.int_rect.clazz, "left", "I");
        runtime.int_rect.top_field = env->GetFieldID(runtime.int_rect.clazz, "top", "I");
        runtime.int_rect.right_field = env->GetFieldID(runtime.int_rect.clazz, "right", "I");
        runtime.int_rect.bottom_field = env->GetFieldID(runtime.int_rect.clazz, "bottom", "I");

        // Retrieve information about the Vec4 class
        runtime.vec4.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/math/Vec4")));
        runtime.vec4.constructor = env->GetMethodID(runtime.vec4.clazz, "<init>", "(FFFF)V");

        // Retrieve information about the UltralightSurface class
        runtime.ultralight_surface.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/UltralightSurface")));
        runtime.ultralight_surface.constructor = env->GetMethodID(
            runtime.ultralight_surface.clazz, "<init>", "(Lcom/labymedia/ultralight/UltralightView;J)V");

        // Register the native methods for the UltralightSurface class
        env->RegisterNatives(
            runtime.ultralight_surface.clazz,
            runtime.ultralight_surface.native_methods.data(),
            runtime.ultralight_surface.native_methods.size());

        // Retrieve information about the UltralightBitmapSurface class
        runtime.ultralight_bitmap_surface.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/bitmap/UltralightBitmapSurface")));
        runtime.ultralight_bitmap_surface.constructor = env->GetMethodID(
            runtime.ultralight_bitmap_surface.clazz, "<init>", "(Lcom/labymedia/ultralight/UltralightView;J)V");

        // Register the native methods for the UltralightBitmapSurface class
        env->RegisterNatives(
            runtime.ultralight_bitmap_surface.clazz,
            runtime.ultralight_bitmap_surface.native_methods.data(),
            runtime.ultralight_bitmap_surface.native_methods.size());

        // Retrieve information about the UltralightBitmapFormat enum
        runtime.ultralight_bitmap_format.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/bitmap/UltralightBitmapFormat")));
        if(!runtime.ultralight_bitmap_format.constants
                .init(env, "com/labymedia/ultralight/bitmap/UltralightBitmapFormat")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightBitmap class
        runtime.ultralight_bitmap.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/bitmap/UltralightBitmap")));
        runtime.ultralight_bitmap.constructor =
            env->GetMethodID(runtime.ultralight_bitmap.clazz, "<init>", "(Lcom/labymedia/ultralight/ffi/RefPtr;)V");

        // Register the native methods for the UltralightBitmap class
        env->RegisterNatives(
            runtime.ultralight_bitmap.clazz,
            runtime.ultralight_bitmap.native_methods.data(),
            runtime.ultralight_bitmap.native_methods.size());

        // Retrieve information about the UltralightCommandList class
        runtime.ultralight_render_target.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/render/UltralightRenderTarget")));
        runtime.ultralight_render_target.constructor = env->GetMethodID(
            runtime.ultralight_render_target.clazz,
            "<init>",
            "("
            "Z"
            "J"
            "J"
            "J"
            "J"
            "J"
            "Lcom/labymedia/ultralight/bitmap/UltralightBitmapFormat;"
            "[F"
            "J"
            ")V");

        // Retrieve information about the UltralightCommand class
        runtime.ultralight_command.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/render/UltralightCommand")));
        runtime.ultralight_command.constructor = env->GetMethodID(
            runtime.ultralight_command.clazz,
            "<init>",
            "("
            "Lcom/labymedia/ultralight/plugin/render/UltralightCommandType;"
            "Lcom/labymedia/ultralight/plugin/render/UltralightGPUState;"
            "J"
            "J"
            "J"
            ")V");

        // Retrieve information about the UltralightCommandType enum
        runtime.ultralight_command_type.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/render/UltralightCommandType")));
        if(!runtime.ultralight_command_type.constants
                .init(env, "com/labymedia/ultralight/plugin/render/UltralightCommandType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightGPUState class
        runtime.ultralight_gpu_state.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/render/UltralightGPUState")));
        runtime.ultralight_gpu_state.constructor = env->GetMethodID(
            runtime.ultralight_gpu_state.clazz,
            "<init>",
            "("
            "J"
            "J"
            "Lcom/labymedia/ultralight/math/UltralightMatrix4x4;"
            "Z"
            "Z"
            "Lcom/labymedia/ultralight/plugin/render/UltralightShaderType;"
            "J"
            "J"
            "J"
            "J"
            "[F"
            "[Lcom/labymedia/ultralight/math/Vec4;"
            "S"
            "[Lcom/labymedia/ultralight/math/UltralightMatrix4x4;"
            "Z"
            "Lcom/labymedia/ultralight/math/IntRect;"
            ")V");

        // Retrieve information about the UltralightShaderType enum
        runtime.ultralight_shader_type.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/render/UltralightShaderType")));
        if(!runtime.ultralight_shader_type.constants
                .init(env, "com/labymedia/ultralight/plugin/render/UltralightShaderType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightIndexBuffer class
        runtime.ultralight_indexbuffer.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/render/UltralightIndexBuffer")));
        runtime.ultralight_indexbuffer
            .constructor = env->GetMethodID(runtime.ultralight_indexbuffer.clazz, "<init>", "(Ljava/nio/ByteBuffer;)V");

        // Retrieve information about the UltralightVertexBuffer class
        runtime.ultralight_vertexbuffer.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/render/UltralightVertexBuffer")));
        runtime.ultralight_vertexbuffer.constructor = env->GetMethodID(
            runtime.ultralight_vertexbuffer.clazz,
            "<init>",
            "(Lcom/labymedia/ultralight/plugin/render/UltralightVertexBufferFormat;Ljava/nio/ByteBuffer;)V");

        // Retrieve information about the UltralightVertexBufferFormat enum
        runtime.ultralight_vertexbuffer_format.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/render/UltralightVertexBufferFormat")));
        if(!runtime.ultralight_vertexbuffer_format.constants
                .init(env, "com/labymedia/ultralight/plugin/render/UltralightVertexBufferFormat")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightRenderBuffer class
        runtime.ultralight_render_buffer.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/render/UltralightRenderBuffer")));
        runtime.ultralight_render_buffer
            .constructor = env->GetMethodID(runtime.ultralight_render_buffer.clazz, "<init>", "(JJJZZ)V");

        // Retrieve information about the UltralightGPUDriver class
        runtime.ultralight_gpu_driver.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/render/UltralightGPUDriver")));
        runtime.ultralight_gpu_driver.begin_synchronize_method =
            env->GetMethodID(runtime.ultralight_gpu_driver.clazz, "beginSynchronize", "()V");
        runtime.ultralight_gpu_driver
            .end_synchronize_method = env->GetMethodID(runtime.ultralight_gpu_driver.clazz, "endSynchronize", "()V");
        runtime.ultralight_gpu_driver
            .next_texture_id_method = env->GetMethodID(runtime.ultralight_gpu_driver.clazz, "nextTextureId", "()J");
        runtime.ultralight_gpu_driver.create_texture_method = env->GetMethodID(
            runtime.ultralight_gpu_driver.clazz,
            "createTexture",
            "(JLcom/labymedia/ultralight/bitmap/UltralightBitmap;)V");
        runtime.ultralight_gpu_driver.update_texture_method = env->GetMethodID(
            runtime.ultralight_gpu_driver.clazz,
            "updateTexture",
            "(JLcom/labymedia/ultralight/bitmap/UltralightBitmap;)V");
        runtime.ultralight_gpu_driver
            .destroy_texture_method = env->GetMethodID(runtime.ultralight_gpu_driver.clazz, "destroyTexture", "(J)V");
        runtime.ultralight_gpu_driver.next_render_buffer_id_method =
            env->GetMethodID(runtime.ultralight_gpu_driver.clazz, "nextRenderBufferId", "()J");
        runtime.ultralight_gpu_driver.create_render_buffer_method = env->GetMethodID(
            runtime.ultralight_gpu_driver.clazz,
            "createRenderBuffer",
            "(JLcom/labymedia/ultralight/plugin/render/UltralightRenderBuffer;)V");
        runtime.ultralight_gpu_driver.destroy_render_buffer_method =
            env->GetMethodID(runtime.ultralight_gpu_driver.clazz, "destroyRenderBuffer", "(J)V");
        runtime.ultralight_gpu_driver
            .next_geometry_id_method = env->GetMethodID(runtime.ultralight_gpu_driver.clazz, "nextGeometryId", "()J");
        runtime.ultralight_gpu_driver.create_geometry_method = env->GetMethodID(
            runtime.ultralight_gpu_driver.clazz,
            "createGeometry",
            "(JLcom/labymedia/ultralight/plugin/render/UltralightVertexBuffer;Lcom/labymedia/ultralight/plugin/render/UltralightIndexBuffer;)V");
        runtime.ultralight_gpu_driver.update_geometry_method = env->GetMethodID(
            runtime.ultralight_gpu_driver.clazz,
            "updateGeometry",
            "(JLcom/labymedia/ultralight/plugin/render/UltralightVertexBuffer;Lcom/labymedia/ultralight/plugin/render/UltralightIndexBuffer;)V");
        runtime.ultralight_gpu_driver
            .destroy_geometry_method = env->GetMethodID(runtime.ultralight_gpu_driver.clazz, "destroyGeometry", "(J)V");
        runtime.ultralight_gpu_driver.update_command_list_method = env->GetMethodID(
            runtime.ultralight_gpu_driver.clazz,
            "updateCommandList",
            "([Lcom/labymedia/ultralight/plugin/render/UltralightCommand;)V");

        runtime.float_array.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("[F")));

        // Retrieve information about the UltralightLoadListener
        runtime.ultralight_load_listener.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/loading/UltralightLoadListener")));
        runtime.ultralight_load_listener.on_begin_loading_method =
            env->GetMethodID(runtime.ultralight_load_listener.clazz, "onBeginLoading", "(JZLjava/lang/String;)V");
        runtime.ultralight_load_listener.on_finish_loading_method =
            env->GetMethodID(runtime.ultralight_load_listener.clazz, "onFinishLoading", "(JZLjava/lang/String;)V");
        runtime.ultralight_load_listener.on_fail_loading_method = env->GetMethodID(
            runtime.ultralight_load_listener.clazz,
            "onFailLoading",
            "(JZLjava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V");
        runtime.ultralight_load_listener.on_update_history_method =
            env->GetMethodID(runtime.ultralight_load_listener.clazz, "onUpdateHistory", "()V");
        runtime.ultralight_load_listener.on_window_object_ready_method =
            env->GetMethodID(runtime.ultralight_load_listener.clazz, "onWindowObjectReady", "(JZLjava/lang/String;)V");
        runtime.ultralight_load_listener.on_dom_ready_method =
            env->GetMethodID(runtime.ultralight_load_listener.clazz, "onDOMReady", "(JZLjava/lang/String;)V");

        // Retrieve information about the UltralightKey class
        runtime.ultralight_key.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/input/UltralightKey")));
        runtime.ultralight_key.id_field = env->GetFieldID(runtime.ultralight_key.clazz, "id", "I");

        // Retrieve information about the UltralightKeyEvent class
        runtime.ultralight_key_event.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/input/UltralightKeyEvent")));
        runtime.ultralight_key_event.type_field = env->GetFieldID(
            runtime.ultralight_key_event.clazz, "type", "Lcom/labymedia/ultralight/input/UltralightKeyEventType;");
        runtime.ultralight_key_event
            .modifiers_field = env->GetFieldID(runtime.ultralight_key_event.clazz, "modifiers", "I");
        runtime.ultralight_key_event.virtual_key_code_field = env->GetFieldID(
            runtime.ultralight_key_event.clazz, "virtualKeyCode", "Lcom/labymedia/ultralight/input/UltralightKey;");
        runtime.ultralight_key_event
            .native_key_code_field = env->GetFieldID(runtime.ultralight_key_event.clazz, "nativeKeyCode", "I");
        runtime.ultralight_key_event.key_identifier_field =
            env->GetFieldID(runtime.ultralight_key_event.clazz, "keyIdentifier", "Ljava/lang/String;");
        runtime.ultralight_key_event
            .text_field = env->GetFieldID(runtime.ultralight_key_event.clazz, "text", "Ljava/lang/String;");
        runtime.ultralight_key_event.unmodified_text_field =
            env->GetFieldID(runtime.ultralight_key_event.clazz, "unmodifiedText", "Ljava/lang/String;");
        runtime.ultralight_key_event
            .is_keypad_field = env->GetFieldID(runtime.ultralight_key_event.clazz, "isKeypad", "Z");
        runtime.ultralight_key_event
            .is_auto_repeat_field = env->GetFieldID(runtime.ultralight_key_event.clazz, "isAutoRepeat", "Z");
        runtime.ultralight_key_event
            .is_system_key_field = env->GetFieldID(runtime.ultralight_key_event.clazz, "isSystemKey", "Z");

        // Register native methods for the UltralightKeyEvent class
        env->RegisterNatives(
            runtime.ultralight_key_event.clazz,
            runtime.ultralight_key_event.native_methods.data(),
            runtime.ultralight_key_event.native_methods.size());

        // Retrieve information about the UltralightKeyEventType enum
        runtime.ultralight_key_event_type.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/input/UltralightKeyEventType")));
        if(!runtime.ultralight_key_event_type.constants
                .init(env, "com/labymedia/ultralight/input/UltralightKeyEventType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightMouseEvent class
        runtime.ultralight_mouse_event.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/input/UltralightMouseEvent")));
        runtime.ultralight_mouse_event.type_field = env->GetFieldID(
            runtime.ultralight_mouse_event.clazz, "type", "Lcom/labymedia/ultralight/input/UltralightMouseEventType;");
        runtime.ultralight_mouse_event.x_field = env->GetFieldID(runtime.ultralight_mouse_event.clazz, "x", "I");
        runtime.ultralight_mouse_event.y_field = env->GetFieldID(runtime.ultralight_mouse_event.clazz, "y", "I");
        runtime.ultralight_mouse_event.button_field = env->GetFieldID(
            runtime.ultralight_mouse_event.clazz,
            "button",
            "Lcom/labymedia/ultralight/input/UltralightMouseEventButton;");

        // Retrieve information about the UltralightMouseEventButton enum
        runtime.ultralight_mouse_event_button.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/input/UltralightMouseEventButton")));
        if(!runtime.ultralight_mouse_event_button.constants
                .init(env, "com/labymedia/ultralight/input/UltralightMouseEventButton")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightMouseEventType enum
        runtime.ultralight_mouse_event_type.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/input/UltralightMouseEventType")));
        if(!runtime.ultralight_mouse_event_type.constants
                .init(env, "com/labymedia/ultralight/input/UltralightMouseEventType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightScrollEvent class
        runtime.ultralight_scroll_event.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/input/UltralightScrollEvent")));
        runtime.ultralight_scroll_event.type_field = env->GetFieldID(
            runtime.ultralight_scroll_event.clazz,
            "type",
            "Lcom/labymedia/ultralight/input/UltralightScrollEventType;");
        runtime.ultralight_scroll_event
            .delta_x_field = env->GetFieldID(runtime.ultralight_scroll_event.clazz, "deltaX", "I");
        runtime.ultralight_scroll_event
            .delta_y_field = env->GetFieldID(runtime.ultralight_scroll_event.clazz, "deltaY", "I");

        // Retrieve information about the UltralightScrollEventType enum
        runtime.ultralight_scroll_event_type.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/input/UltralightScrollEventType")));
        if(!runtime.ultralight_scroll_event_type.constants
                .init(env, "com/labymedia/ultralight/input/UltralightScrollEventType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightCursor enum
        runtime.ultralight_cursor.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/input/UltralightCursor")));
        if(!runtime.ultralight_cursor.constants.init(env, "com/labymedia/ultralight/input/UltralightCursor")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the MessageLevel enum
        runtime.message_level.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/view/MessageLevel")));
        if(!runtime.message_level.constants.init(env, "com/labymedia/ultralight/plugin/view/MessageLevel")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the MessageSource enum
        runtime.message_source.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/view/MessageSource")));
        if(!runtime.message_source.constants.init(env, "com/labymedia/ultralight/plugin/view/MessageSource")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightViewListener class
        runtime.ultralight_view_listener.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/view/UltralightViewListener")));
        runtime.ultralight_view_listener.on_change_title_method =
            env->GetMethodID(runtime.ultralight_view_listener.clazz, "onChangeTitle", "(Ljava/lang/String;)V");
        runtime.ultralight_view_listener.on_change_url_method =
            env->GetMethodID(runtime.ultralight_view_listener.clazz, "onChangeURL", "(Ljava/lang/String;)V");
        runtime.ultralight_view_listener.on_change_tooltip_method =
            env->GetMethodID(runtime.ultralight_view_listener.clazz, "onChangeTooltip", "(Ljava/lang/String;)V");
        runtime.ultralight_view_listener.on_change_cursor_method = env->GetMethodID(
            runtime.ultralight_view_listener.clazz,
            "onChangeCursor",
            "(Lcom/labymedia/ultralight/input/UltralightCursor;)V");
        runtime.ultralight_view_listener.on_add_console_message_method = env->GetMethodID(
            runtime.ultralight_view_listener.clazz,
            "onAddConsoleMessage",
            "("
            "Lcom/labymedia/ultralight/plugin/view/MessageSource;"
            "Lcom/labymedia/ultralight/plugin/view/MessageLevel;"
            "Ljava/lang/String;"
            "J"
            "J"
            "Ljava/lang/String;"
            ")V");
        runtime.ultralight_view_listener.on_create_child_view_method = env->GetMethodID(
            runtime.ultralight_view_listener.clazz,
            "onCreateChildView",
            "("
            "Ljava/lang/String;"
            "Ljava/lang/String;"
            "Z"
            "Lcom/labymedia/ultralight/math/IntRect;"
            ")Lcom/labymedia/ultralight/UltralightView;");

        // Retrieve information about the Runnable class
        runtime.runnable.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("java/lang/Runnable")));
        runtime.runnable.run_method = env->GetMethodID(runtime.runnable.clazz, "run", "()V");

        // Retrieve information about the UltralightFileSystem class
        runtime.ultralight_file_system.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/filesystem/UltralightFileSystem")));
        runtime.ultralight_file_system.file_exists_method =
            env->GetMethodID(runtime.ultralight_file_system.clazz, "fileExists", "(Ljava/lang/String;)Z");
        runtime.ultralight_file_system
            .get_file_size_method = env->GetMethodID(runtime.ultralight_file_system.clazz, "getFileSize", "(J)J");
        runtime.ultralight_file_system.get_file_mime_type_method = env->GetMethodID(
            runtime.ultralight_file_system.clazz, "getFileMimeType", "(Ljava/lang/String;)Ljava/lang/String;");
        runtime.ultralight_file_system.open_file_method =
            env->GetMethodID(runtime.ultralight_file_system.clazz, "openFile", "(Ljava/lang/String;Z)J");
        runtime.ultralight_file_system
            .close_file_method = env->GetMethodID(runtime.ultralight_file_system.clazz, "closeFile", "(J)V");
        runtime.ultralight_file_system.read_from_file_method =
            env->GetMethodID(runtime.ultralight_file_system.clazz, "readFromFile", "(JLjava/nio/ByteBuffer;J)J");

        runtime.ultralight_clipboard.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/plugin/clipboard/UltralightClipboard")));
        runtime.ultralight_clipboard
            .clear_method = env->GetMethodID(runtime.ultralight_clipboard.clazz, "clear", "()V");
        runtime.ultralight_clipboard.read_plain_text_method =
            env->GetMethodID(runtime.ultralight_clipboard.clazz, "readPlainText", "()Ljava/lang/String;");
        runtime.ultralight_clipboard.write_plain_text_method =
            env->GetMethodID(runtime.ultralight_clipboard.clazz, "writePlainText", "(Ljava/lang/String;)V");

        // Retrieve information about the JavascriptContext class
        runtime.javascript_context.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptContext")));
        runtime.javascript_context.constructor = env->GetMethodID(
            runtime.javascript_context.clazz,
            "<init>",
            "(JLcom/labymedia/ultralight/javascript/JavascriptContextLock;)V");

        // Register native methods for the JavascriptContext class
        env->RegisterNatives(
            runtime.javascript_context.clazz,
            runtime.javascript_context.native_methods.data(),
            runtime.javascript_context.native_methods.size());

        // Retrieve information about the JavascriptContextLock
        runtime.javascript_context_lock.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptContextLock")));
        runtime.javascript_context_lock
            .constructor = env->GetMethodID(runtime.javascript_context_lock.clazz, "<init>", "(J)V");
        runtime.javascript_context_lock
            .unlock_method = env->GetMethodID(runtime.javascript_context_lock.clazz, "unlock", "()V");

        // Register native methods for the JavascriptContextLock class
        env->RegisterNatives(
            runtime.javascript_context_lock.clazz,
            runtime.javascript_context_lock.native_methods.data(),
            runtime.javascript_context_lock.native_methods.size());

        // Retrieve information about the JavascriptGlobalContext class
        runtime.javascript_global_context.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptGlobalContext")));
        runtime.javascript_global_context.constructor = env->GetMethodID(
            runtime.javascript_global_context.clazz,
            "<init>",
            "(JLcom/labymedia/ultralight/javascript/JavascriptContextLock;)V");

        // Register native methods for the JavascriptGlobalContext class
        env->RegisterNatives(
            runtime.javascript_global_context.clazz,
            runtime.javascript_global_context.native_methods.data(),
            runtime.javascript_global_context.native_methods.size());

        // Retrieve information about the JavascriptLockedObject interface
        runtime.javascript_locked_object.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptLockedObject")));
        runtime.javascript_locked_object.get_lock_method = env->GetMethodID(
            runtime.javascript_locked_object.clazz,
            "getLock",
            "()Lcom/labymedia/ultralight/javascript/JavascriptContextLock;");
        runtime.javascript_locked_object.get_context_handle_method =
            env->GetMethodID(runtime.javascript_locked_object.clazz, "getContextHandle", "()J");
        runtime.javascript_locked_object
            .get_lock_handle_method = env->GetMethodID(runtime.javascript_locked_object.clazz, "getLockHandle", "()J");

        // Retrieve information about the JavascriptProtectedValue class
        runtime.javascript_protected_value.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptProtectedValue")));
        runtime.javascript_protected_value
            .constructor = env->GetMethodID(runtime.javascript_protected_value.clazz, "<init>", "(J)V");

        // Retrieve information about the JavascriptValue class
        runtime.javascript_value.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptValue")));
        runtime.javascript_value.constructor = env->GetMethodID(
            runtime.javascript_value.clazz,
            "<init>",
            "(JLcom/labymedia/ultralight/javascript/JavascriptContextLock;)V");

        // Register native methods for the JavascriptValue class
        env->RegisterNatives(
            runtime.javascript_value.clazz,
            runtime.javascript_value.native_methods.data(),
            runtime.javascript_value.native_methods.size());

        // Retrieve information about the JavascriptObject class
        runtime.javascript_object.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptObject")));
        runtime.javascript_object.constructor = env->GetMethodID(
            runtime.javascript_object.clazz,
            "<init>",
            "(JLcom/labymedia/ultralight/javascript/JavascriptContextLock;)V");

        // Register native methods for the JavascriptObject class
        env->RegisterNatives(
            runtime.javascript_object.clazz,
            runtime.javascript_object.native_methods.data(),
            runtime.javascript_object.native_methods.size());

        // Retrieve information about the JavascriptType enum
        runtime.javascript_type.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptType")));
        if(!runtime.javascript_type.constants.init(env, "com/labymedia/ultralight/javascript/JavascriptType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the JavascriptTypedArrayType enum
        runtime.javascript_typed_array_type.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptTypedArrayType")));
        if(!runtime.javascript_typed_array_type.constants
                .init(env, "com/labymedia/ultralight/javascript/JavascriptTypedArrayType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the JavascriptClass class
        runtime.javascript_class.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptClass")));
        runtime.javascript_class.constructor = env->GetMethodID(runtime.javascript_class.clazz, "<init>", "(J)V");

        // Register native methods for the JavascriptClass class
        env->RegisterNatives(
            runtime.javascript_class.clazz,
            runtime.javascript_class.native_methods.data(),
            runtime.javascript_class.native_methods.size());

        // Retrieve information about the JavascriptClassDefinition class
        runtime.javascript_class_definition.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptClassDefinition")));

        // Register native methods for the JavascriptClassDefinition class
        env->RegisterNatives(
            runtime.javascript_class_definition.clazz,
            runtime.javascript_class_definition.native_methods.data(),
            runtime.javascript_class_definition.native_methods.size());

        // Retrieve information about the JavascriptObjectConstructor interface
        runtime.javascript_object_constructor.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/labymedia/ultralight/javascript/interop/JavascriptObjectConstructor")));
        runtime.javascript_object_constructor.call_as_javascript_constructor_method = env->GetMethodID(
            runtime.javascript_object_constructor.clazz,
            "callAsJavascriptConstructor",
            "("
            "Lcom/labymedia/ultralight/javascript/JavascriptContext;"
            "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
            "[Lcom/labymedia/ultralight/javascript/JavascriptValue;"
            ")Lcom/labymedia/ultralight/javascript/JavascriptObject;");

        // Retrieve information about the JavascriptObjectFinalizer interface
        runtime.javascript_object_finalizer.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/interop/JavascriptObjectFinalizer")));
        runtime.javascript_object_finalizer.finalize_javascript_object = env->GetMethodID(
            runtime.javascript_object_finalizer.clazz,
            "finalizeJavascriptObject",
            "(Lcom/labymedia/ultralight/javascript/JavascriptObject;)V");

        // Retrieve information about the JavascriptObjectFunction interface
        runtime.javascript_object_function.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/interop/JavascriptObjectFunction")));
        runtime.javascript_object_function.call_as_javascript_function = env->GetMethodID(
            runtime.javascript_object_function.clazz,
            "callAsJavascriptFunction",
            "("
            "Lcom/labymedia/ultralight/javascript/JavascriptContext;"
            "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
            "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
            "[Lcom/labymedia/ultralight/javascript/JavascriptValue;"
            ")Lcom/labymedia/ultralight/javascript/JavascriptValue;");

        // Retrieve information about the JavascriptObjectHasInstanceTester interface
        runtime.javascript_object_has_instance_tester.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/labymedia/ultralight/javascript/interop/JavascriptObjectHasInstanceTester")));
        runtime.javascript_object_has_instance_tester.has_javascript_instance_method = env->GetMethodID(
            runtime.javascript_object_has_instance_tester.clazz,
            "hasJavascriptInstance",
            "("
            "Lcom/labymedia/ultralight/javascript/JavascriptContext;"
            "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
            "Lcom/labymedia/ultralight/javascript/JavascriptValue;"
            ")Z");

        // Retrieve information about the JavascriptObjectHasPropertyTester interface
        runtime.javascript_object_has_property_tester.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/labymedia/ultralight/javascript/interop/JavascriptObjectHasPropertyTester")));
        runtime.javascript_object_has_property_tester.has_javascript_property_method = env->GetMethodID(
            runtime.javascript_object_has_property_tester.clazz,
            "hasJavascriptProperty",
            "("
            "Lcom/labymedia/ultralight/javascript/JavascriptContext;"
            "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
            "Ljava/lang/String;"
            ")Z");

        // Retrieve information about the JavascriptObjectInitializer interface
        runtime.javascript_object_initializer.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/labymedia/ultralight/javascript/interop/JavascriptObjectInitializer")));
        runtime.javascript_object_initializer.initialize_javascript_object_method = env->GetMethodID(
            runtime.javascript_object_initializer.clazz,
            "initializeJavascriptObject",
            "("
            "Lcom/labymedia/ultralight/javascript/JavascriptContext;"
            "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
            ")V");

        // Retrieve information about the JavascriptObjectPropertyDeleter interface
        runtime.javascript_object_property_deleter.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/labymedia/ultralight/javascript/interop/JavascriptObjectPropertyDeleter")));
        runtime.javascript_object_property_deleter.delete_javascript_property_method = env->GetMethodID(
            runtime.javascript_object_property_deleter.clazz,
            "deleteJavascriptProperty",
            "("
            "Lcom/labymedia/ultralight/javascript/JavascriptContext;"
            "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
            "Ljava/lang/String;"
            ")Z");

        // Retrieve information about the JavascriptObjectPropertyGetter interface
        runtime.javascript_object_property_getter.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/labymedia/ultralight/javascript/interop/JavascriptObjectPropertyGetter")));
        runtime.javascript_object_property_getter.get_javascript_property_method = env->GetMethodID(
            runtime.javascript_object_property_getter.clazz,
            "getJavascriptProperty",
            "("
            "Lcom/labymedia/ultralight/javascript/JavascriptContext;"
            "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
            "Ljava/lang/String;"
            ")Lcom/labymedia/ultralight/javascript/JavascriptValue;");

        // Retrieve information about the JavascriptObjectPropertyNamesCollector interface
        runtime.javascript_object_property_names_collector.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/labymedia/ultralight/javascript/interop/JavascriptObjectPropertyNamesCollector")));
        runtime.javascript_object_property_names_collector.collect_javascript_property_names_method = env->GetMethodID(
            runtime.javascript_object_property_names_collector.clazz,
            "collectJavascriptPropertyNames",
            "("
            "Lcom/labymedia/ultralight/javascript/JavascriptContext;"
            "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
            ")[Ljava/lang/String;");

        // Retrieve information about the JavascriptObjectPropertySetter interface
        runtime.javascript_object_property_setter.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/labymedia/ultralight/javascript/interop/JavascriptObjectPropertySetter")));
        runtime.javascript_object_property_setter.set_javascript_property_method = env->GetMethodID(
            runtime.javascript_object_property_setter.clazz,
            "setJavascriptProperty",
            "("
            "Lcom/labymedia/ultralight/javascript/JavascriptContext;"
            "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
            "Ljava/lang/String;"
            "Lcom/labymedia/ultralight/javascript/JavascriptValue;"
            ")Z");

        // Retrieve information about the JavascriptObjectToTypeConverter interface
        runtime.javascript_object_to_type_converter.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("com/labymedia/ultralight/javascript/interop/JavascriptObjectToTypeConverter")));
        runtime.javascript_object_to_type_converter.convert_to_javascript_type_method = env->GetMethodID(
            runtime.javascript_object_to_type_converter.clazz,
            "convertToJavascriptType",
            "("
            "Lcom/labymedia/ultralight/javascript/JavascriptContext;"
            "Lcom/labymedia/ultralight/javascript/JavascriptObject;"
            "Lcom/labymedia/ultralight/javascript/JavascriptType;"
            ")Lcom/labymedia/ultralight/javascript/JavascriptValue;");

        // Retrieve information about the Throwable class
        runtime.throwable.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("java/lang/Throwable")));
        runtime.throwable
            .get_message_method = env->GetMethodID(runtime.throwable.clazz, "getMessage", "()Ljava/lang/String;");

        // Retrieve information about the JavascriptException class
        runtime.javascript_exception.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptException")));
        runtime.javascript_exception.constructor = env->GetMethodID(
            runtime.javascript_exception.clazz,
            "<init>",
            "(Ljava/lang/String;Lcom/labymedia/ultralight/javascript/JavascriptValue;)V");

        // Retrieve information about the JavascriptEvaluationException class
        runtime.javascript_evaluation_exception.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("com/labymedia/ultralight/javascript/JavascriptEvaluationException")));

        // Retrieve information about the NullPointerException class
        runtime.null_pointer_exception.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("java/lang/NullPointerException")));

        // Retrieve information about the IllegalStateException class
        runtime.illegal_state_exception.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("java/lang/IllegalStateException")));

        // Retrieve information about the IllegalArgumentException class
        runtime.illegal_argument_exception.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("java/lang/IllegalArgumentException")));

        return JNI_VERSION_1_8;
    }

    void destruct(JavaVM *vm) {
        // Retrieve a JNI environment
        JNIEnv *env;
        vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_8);

        UltralightViewJNI::clean_up();

        delete runtime.bridged_logger;

        env->DeleteGlobalRef(runtime.illegal_argument_exception.clazz);
        env->DeleteGlobalRef(runtime.illegal_state_exception.clazz);
        env->DeleteGlobalRef(runtime.null_pointer_exception.clazz);
        env->DeleteGlobalRef(runtime.javascript_evaluation_exception.clazz);
        env->DeleteGlobalRef(runtime.javascript_exception.clazz);
        env->DeleteGlobalRef(runtime.throwable.clazz);
        env->DeleteGlobalRef(runtime.javascript_object_to_type_converter.clazz);
        env->DeleteGlobalRef(runtime.javascript_object_property_setter.clazz);
        env->DeleteGlobalRef(runtime.javascript_object_property_names_collector.clazz);
        env->DeleteGlobalRef(runtime.javascript_object_property_getter.clazz);
        env->DeleteGlobalRef(runtime.javascript_object_property_deleter.clazz);
        env->DeleteGlobalRef(runtime.javascript_object_initializer.clazz);
        env->DeleteGlobalRef(runtime.javascript_object_has_property_tester.clazz);
        env->DeleteGlobalRef(runtime.javascript_object_has_instance_tester.clazz);
        env->DeleteGlobalRef(runtime.javascript_object_function.clazz);
        env->DeleteGlobalRef(runtime.javascript_object_finalizer.clazz);
        env->DeleteGlobalRef(runtime.javascript_object_constructor.clazz);
        env->UnregisterNatives(runtime.javascript_class_definition.clazz);
        env->DeleteGlobalRef(runtime.javascript_class_definition.clazz);
        env->UnregisterNatives(runtime.javascript_class.clazz);
        env->DeleteGlobalRef(runtime.javascript_class.clazz);
        runtime.javascript_typed_array_type.constants.clear(env);
        env->DeleteGlobalRef(runtime.javascript_typed_array_type.clazz);
        runtime.javascript_type.constants.clear(env);
        env->DeleteGlobalRef(runtime.javascript_type.clazz);
        env->DeleteLocalRef(runtime.javascript_object.clazz);
        env->UnregisterNatives(runtime.javascript_value.clazz);
        env->DeleteGlobalRef(runtime.javascript_value.clazz);
        env->DeleteGlobalRef(runtime.javascript_protected_value.clazz);
        env->DeleteGlobalRef(runtime.javascript_locked_object.clazz);
        env->UnregisterNatives(runtime.javascript_global_context.clazz);
        env->DeleteGlobalRef(runtime.javascript_global_context.clazz);
        env->UnregisterNatives(runtime.javascript_context_lock.clazz);
        env->DeleteGlobalRef(runtime.javascript_context_lock.clazz);
        env->UnregisterNatives(runtime.javascript_context.clazz);
        env->DeleteGlobalRef(runtime.javascript_context.clazz);
        env->DeleteGlobalRef(runtime.ultralight_clipboard.clazz);
        env->DeleteGlobalRef(runtime.ultralight_file_system.clazz);
        env->DeleteGlobalRef(runtime.runnable.clazz);
        env->DeleteGlobalRef(runtime.ultralight_view_listener.clazz);
        runtime.message_source.constants.clear(env);
        env->DeleteGlobalRef(runtime.message_source.clazz);
        runtime.message_level.constants.clear(env);
        env->DeleteGlobalRef(runtime.message_level.clazz);
        runtime.ultralight_cursor.constants.clear(env);
        env->DeleteGlobalRef(runtime.ultralight_cursor.clazz);
        runtime.ultralight_scroll_event_type.constants.clear(env);
        env->DeleteGlobalRef(runtime.ultralight_scroll_event_type.clazz);
        env->DeleteGlobalRef(runtime.ultralight_scroll_event.clazz);
        runtime.ultralight_mouse_event_type.constants.clear(env);
        env->DeleteGlobalRef(runtime.ultralight_mouse_event_type.clazz);
        runtime.ultralight_mouse_event_button.constants.clear(env);
        env->DeleteGlobalRef(runtime.ultralight_mouse_event_button.clazz);
        env->DeleteGlobalRef(runtime.ultralight_mouse_event.clazz);
        runtime.ultralight_key_event_type.constants.clear(env);
        env->DeleteGlobalRef(runtime.ultralight_key_event_type.clazz);
        env->UnregisterNatives(runtime.ultralight_key_event.clazz);
        env->DeleteGlobalRef(runtime.ultralight_key_event.clazz);
        env->DeleteGlobalRef(runtime.ultralight_key.clazz);
        env->DeleteGlobalRef(runtime.ultralight_render_target.clazz);
        runtime.ultralight_shader_type.constants.clear(env);
        env->DeleteGlobalRef(runtime.ultralight_shader_type.clazz);
        runtime.ultralight_command_type.constants.clear(env);
        env->DeleteGlobalRef(runtime.ultralight_command_type.clazz);
        env->DeleteGlobalRef(runtime.ultralight_command.clazz);
        env->DeleteGlobalRef(runtime.ultralight_gpu_state.clazz);
        env->DeleteGlobalRef(runtime.ultralight_indexbuffer.clazz);
        env->DeleteGlobalRef(runtime.ultralight_vertexbuffer.clazz);
        runtime.ultralight_vertexbuffer_format.constants.clear(env);
        env->DeleteGlobalRef(runtime.ultralight_vertexbuffer_format.clazz);
        env->DeleteGlobalRef(runtime.ultralight_render_buffer.clazz);
        env->DeleteGlobalRef(runtime.ultralight_gpu_driver.clazz);
        env->DeleteGlobalRef(runtime.ultralight_load_listener.clazz);
        env->UnregisterNatives(runtime.ultralight_bitmap.clazz);
        env->DeleteGlobalRef(runtime.ultralight_bitmap.clazz);
        runtime.ultralight_bitmap_format.constants.clear(env);
        env->DeleteGlobalRef(runtime.ultralight_bitmap_format.clazz);
        env->DeleteGlobalRef(runtime.ultralight_bitmap_surface.clazz);
        env->UnregisterNatives(runtime.ultralight_surface.clazz);
        env->DeleteGlobalRef(runtime.ultralight_surface.clazz);
        env->DeleteGlobalRef(runtime.int_rect.clazz);
        env->DeleteGlobalRef(runtime.vec4.clazz);
        env->UnregisterNatives(runtime.ultralight_renderer.clazz);
        env->DeleteGlobalRef(runtime.ultralight_renderer.clazz);
        env->UnregisterNatives(runtime.ref_ptr.clazz);
        env->DeleteGlobalRef(runtime.ref_ptr.clazz);
        runtime.log_level.constants.clear(env);
        env->DeleteGlobalRef(runtime.log_level.clazz);
        env->DeleteGlobalRef(runtime.logger.clazz);
        runtime.font_hinting.constants.clear(env);
        env->DeleteGlobalRef(runtime.font_hinting.clazz);
        runtime.face_winding.constants.clear(env);
        env->DeleteGlobalRef(runtime.face_winding.clazz);
        env->DeleteGlobalRef(runtime.ultralight_view_config.clazz);
        env->DeleteGlobalRef(runtime.ultralight_config.clazz);
        env->DeleteGlobalRef(runtime.ultralight_matrix.clazz);
        env->DeleteGlobalRef(runtime.ultralight_matrix4x4.clazz);
        env->UnregisterNatives(runtime.ultralight_platform.clazz);
        env->DeleteGlobalRef(runtime.ultralight_platform.clazz);
        env->DeleteGlobalRef(runtime.object_with_handle.clazz);
        env->DeleteGlobalRef(runtime.string.clazz);
    }
} // namespace ultralight_java