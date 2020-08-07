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
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/ffi/ObjectWithHandle")));
        runtime.object_with_handle
            .get_handle_method = env->GetMethodID(runtime.object_with_handle.clazz, "getHandle", "()J");

        // Retrieve information about the UltralightPlatform class
        runtime.ultralight_platform.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/UltralightPlatform")));
        runtime.ultralight_platform.constructor = env->GetMethodID(runtime.ultralight_platform.clazz, "<init>", "(J)V");

        // Register the native methods for the UltralightPlatform class
        env->RegisterNatives(
            runtime.ultralight_platform.clazz,
            runtime.ultralight_platform.native_methods.data(),
            runtime.ultralight_platform.native_methods.size());

        // Retrieve information about the UltralightConfig class
        auto ultralight_config_class = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/config/UltralightConfig")));
        runtime.ultralight_config.clazz = ultralight_config_class;

        auto &ultralight_config = runtime.ultralight_config;
        ultralight_config
            .resource_path_field = env->GetFieldID(ultralight_config_class, "resourcePath", "Ljava/lang/String;");
        ultralight_config
            .cache_path_field = env->GetFieldID(ultralight_config_class, "cachePath", "Ljava/lang/String;");
        ultralight_config.use_gpu_renderer_field = env->GetFieldID(ultralight_config_class, "useGpuRenderer", "Z");
        ultralight_config.device_scale_field = env->GetFieldID(ultralight_config_class, "deviceScale", "D");
        ultralight_config.face_winding_field =
            env->GetFieldID(ultralight_config_class, "faceWinding", "Lnet/janrupf/ultralight/config/FaceWinding;");
        ultralight_config.enable_images_field = env->GetFieldID(ultralight_config_class, "enableImages", "Z");
        ultralight_config.enable_javascript_field = env->GetFieldID(ultralight_config_class, "enableJavascript", "Z");
        ultralight_config.font_hinting_field =
            env->GetFieldID(ultralight_config_class, "fontHinting", "Lnet/janrupf/ultralight/config/FontHinting;");
        ultralight_config.font_gamma_field = env->GetFieldID(ultralight_config_class, "fontGamma", "D");
        ultralight_config.font_family_standard_field =
            env->GetFieldID(ultralight_config_class, "fontFamilyStandard", "Ljava/lang/String;");
        ultralight_config.font_family_fixed_field =
            env->GetFieldID(ultralight_config_class, "fontFamilyFixed", "Ljava/lang/String;");
        ultralight_config.font_family_serif_field =
            env->GetFieldID(ultralight_config_class, "fontFamilySerif", "Ljava/lang/String;");
        ultralight_config.font_family_sans_serif_field =
            env->GetFieldID(ultralight_config_class, "fontFamilySansSerif", "Ljava/lang/String;");
        ultralight_config
            .user_agent_field = env->GetFieldID(ultralight_config_class, "userAgent", "Ljava/lang/String;");
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

        // Retrieve information about the FaceWinding enum
        runtime.face_winding.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/config/FaceWinding")));
        if(!runtime.face_winding.constants.init(env, "net/janrupf/ultralight/config/FaceWinding")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the FontHinting enum
        runtime.font_hinting.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/config/FontHinting")));
        if(!runtime.font_hinting.constants.init(env, "net/janrupf/ultralight/config/FontHinting")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightLogger interface
        runtime.logger.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/plugin/logging/UltralightLogger")));
        runtime.logger.log_message_method = env->GetMethodID(
            runtime.logger.clazz,
            "logMessage",
            "(Lnet/janrupf/ultralight/plugin/logging/UltralightLogLevel;Ljava/lang/String;)V");

        // Retrieve information about the UltralightLogLevel enum
        runtime.log_level.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/plugin/logging/UltralightLogLevel")));
        if(!runtime.log_level.constants.init(env, "net/janrupf/ultralight/plugin/logging/UltralightLogLevel")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the RefPtr class
        runtime.ref_ptr.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/ffi/RefPtr")));
        runtime.ref_ptr.constructor = env->GetMethodID(runtime.ref_ptr.clazz, "<init>", "(J)V");

        // Register the native methods for the RefPtr class
        env->RegisterNatives(
            runtime.ref_ptr.clazz, runtime.ref_ptr.native_methods.data(), runtime.ref_ptr.native_methods.size());

        // Retrieve information about the UltralightRenderer class
        runtime.ultralight_renderer.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/UltralightRenderer")));
        runtime.ultralight_renderer.constructor =
            env->GetMethodID(runtime.ultralight_renderer.clazz, "<init>", "(Lnet/janrupf/ultralight/ffi/RefPtr;)V");

        // Register the native methods for the UltralightRenderer class
        env->RegisterNatives(
            runtime.ultralight_renderer.clazz,
            runtime.ultralight_renderer.native_methods.data(),
            runtime.ultralight_renderer.native_methods.size());

        // Retrieve information about the UltralightView class
        runtime.ultralight_view.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/UltralightView")));
        runtime.ultralight_view.constructor =
            env->GetMethodID(runtime.ultralight_view.clazz, "<init>", "(Lnet/janrupf/ultralight/ffi/RefPtr;)V");

        // Register the native methods for the UltralightView class
        env->RegisterNatives(
            runtime.ultralight_view.clazz,
            runtime.ultralight_view.native_methods.data(),
            runtime.ultralight_view.native_methods.size());

        // Retrieve information about the IntRect class
        runtime.int_rect.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/math/IntRect")));
        runtime.int_rect.bounds_constructor = env->GetMethodID(runtime.int_rect.clazz, "<init>", "(IIII)V");
        runtime.int_rect.left_field = env->GetFieldID(runtime.int_rect.clazz, "left", "I");
        runtime.int_rect.top_field = env->GetFieldID(runtime.int_rect.clazz, "top", "I");
        runtime.int_rect.right_field = env->GetFieldID(runtime.int_rect.clazz, "right", "I");
        runtime.int_rect.bottom_field = env->GetFieldID(runtime.int_rect.clazz, "bottom", "I");

        // Retrieve information about the UltralightSurface class
        runtime.ultralight_surface.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/UltralightSurface")));
        runtime.ultralight_surface.constructor =
            env->GetMethodID(runtime.ultralight_surface.clazz, "<init>", "(Lnet/janrupf/ultralight/UltralightView;J)V");

        // Register the native methods for the UltralightSurface class
        env->RegisterNatives(
            runtime.ultralight_surface.clazz,
            runtime.ultralight_surface.native_methods.data(),
            runtime.ultralight_surface.native_methods.size());

        // Retrieve information about the UltralightBitmapSurface class
        runtime.ultralight_bitmap_surface.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/bitmap/UltralightBitmapSurface")));
        runtime.ultralight_bitmap_surface.constructor = env->GetMethodID(
            runtime.ultralight_bitmap_surface.clazz, "<init>", "(Lnet/janrupf/ultralight/UltralightView;J)V");

        // Register the native methods for the UltralightBitmapSurface class
        env->RegisterNatives(
            runtime.ultralight_bitmap_surface.clazz,
            runtime.ultralight_bitmap_surface.native_methods.data(),
            runtime.ultralight_bitmap_surface.native_methods.size());

        // Retrieve information about the UltralightBitmapFormat enum
        runtime.ultralight_bitmap_format.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/bitmap/UltralightBitmapFormat")));
        if(!runtime.ultralight_bitmap_format.constants
                .init(env, "net/janrupf/ultralight/bitmap/UltralightBitmapFormat")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightBitmap class
        runtime.ultralight_bitmap.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/bitmap/UltralightBitmap")));
        runtime.ultralight_bitmap.constructor =
            env->GetMethodID(runtime.ultralight_bitmap.clazz, "<init>", "(Lnet/janrupf/ultralight/ffi/RefPtr;)V");

        // Register the native methods for the UltralightBitmap class
        env->RegisterNatives(
            runtime.ultralight_bitmap.clazz,
            runtime.ultralight_bitmap.native_methods.data(),
            runtime.ultralight_bitmap.native_methods.size());

        // Retrieve information about the UltralightLoadListener
        runtime.ultralight_load_listener.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/plugin/loading/UltralightLoadListener")));
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
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/input/UltralightKey")));
        runtime.ultralight_key.id_field = env->GetFieldID(runtime.ultralight_key.clazz, "id", "I");

        // Retrieve information about the UltralightKeyEvent class
        runtime.ultralight_key_event.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/input/UltralightKeyEvent")));
        runtime.ultralight_key_event.type_field = env->GetFieldID(
            runtime.ultralight_key_event.clazz, "type", "Lnet/janrupf/ultralight/input/UltralightKeyEventType;");
        runtime.ultralight_key_event
            .modifiers_field = env->GetFieldID(runtime.ultralight_key_event.clazz, "modifiers", "I");
        runtime.ultralight_key_event.virtual_key_code_field = env->GetFieldID(
            runtime.ultralight_key_event.clazz, "virtualKeyCode", "Lnet/janrupf/ultralight/input/UltralightKey;");
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
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/input/UltralightKeyEventType")));
        if(!runtime.ultralight_key_event_type.constants
                .init(env, "net/janrupf/ultralight/input/UltralightKeyEventType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightMouseEvent class
        runtime.ultralight_mouse_event.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/input/UltralightMouseEvent")));
        runtime.ultralight_mouse_event.type_field = env->GetFieldID(
            runtime.ultralight_mouse_event.clazz, "type", "Lnet/janrupf/ultralight/input/UltralightMouseEventType;");
        runtime.ultralight_mouse_event.x_field = env->GetFieldID(runtime.ultralight_mouse_event.clazz, "x", "I");
        runtime.ultralight_mouse_event.y_field = env->GetFieldID(runtime.ultralight_mouse_event.clazz, "y", "I");
        runtime.ultralight_mouse_event.button_field = env->GetFieldID(
            runtime.ultralight_mouse_event.clazz,
            "button",
            "Lnet/janrupf/ultralight/input/UltralightMouseEventButton;");

        // Retrieve information about the UltralightMouseEventButton enum
        runtime.ultralight_mouse_event_button.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/input/UltralightMouseEventButton")));
        if(!runtime.ultralight_mouse_event_button.constants
                .init(env, "net/janrupf/ultralight/input/UltralightMouseEventButton")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightMouseEventType enum
        runtime.ultralight_mouse_event_type.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/input/UltralightMouseEventType")));
        if(!runtime.ultralight_mouse_event_type.constants
                .init(env, "net/janrupf/ultralight/input/UltralightMouseEventType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightScrollEvent class
        runtime.ultralight_scroll_event.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/input/UltralightScrollEvent")));
        runtime.ultralight_scroll_event.type_field = env->GetFieldID(
            runtime.ultralight_scroll_event.clazz, "type", "Lnet/janrupf/ultralight/input/UltralightScrollEventType;");
        runtime.ultralight_scroll_event
            .delta_x_field = env->GetFieldID(runtime.ultralight_scroll_event.clazz, "deltaX", "I");
        runtime.ultralight_scroll_event
            .delta_y_field = env->GetFieldID(runtime.ultralight_scroll_event.clazz, "deltaY", "I");

        // Retrieve information about the UltralightScrollEventType enum
        runtime.ultralight_scroll_event_type.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/input/UltralightScrollEventType")));
        if(!runtime.ultralight_scroll_event_type.constants
                .init(env, "net/janrupf/ultralight/input/UltralightScrollEventType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightCursor enum
        runtime.ultralight_cursor.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/input/UltralightCursor")));
        if(!runtime.ultralight_cursor.constants.init(env, "net/janrupf/ultralight/input/UltralightCursor")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the MessageLevel enum
        runtime.message_level.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/plugin/view/MessageLevel")));
        if(!runtime.message_level.constants.init(env, "net/janrupf/ultralight/plugin/view/MessageLevel")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the MessageSource enum
        runtime.message_source.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/plugin/view/MessageSource")));
        if(!runtime.message_source.constants.init(env, "net/janrupf/ultralight/plugin/view/MessageSource")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the UltralightViewListener class
        runtime.ultralight_view_listener.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/plugin/view/UltralightViewListener")));
        runtime.ultralight_view_listener.on_change_title_method =
            env->GetMethodID(runtime.ultralight_view_listener.clazz, "onChangeTitle", "(Ljava/lang/String;)V");
        runtime.ultralight_view_listener.on_change_url_method =
            env->GetMethodID(runtime.ultralight_view_listener.clazz, "onChangeURL", "(Ljava/lang/String;)V");
        runtime.ultralight_view_listener.on_change_tooltip_method =
            env->GetMethodID(runtime.ultralight_view_listener.clazz, "onChangeTooltip", "(Ljava/lang/String;)V");
        runtime.ultralight_view_listener.on_change_cursor_method = env->GetMethodID(
            runtime.ultralight_view_listener.clazz,
            "onChangeCursor",
            "(Lnet/janrupf/ultralight/input/UltralightCursor;)V");
        runtime.ultralight_view_listener.on_add_console_message_method = env->GetMethodID(
            runtime.ultralight_view_listener.clazz,
            "onAddConsoleMessage",
            "("
            "Lnet/janrupf/ultralight/plugin/view/MessageSource;"
            "Lnet/janrupf/ultralight/plugin/view/MessageLevel;"
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
            "Lnet/janrupf/ultralight/math/IntRect;"
            ")Lnet/janrupf/ultralight/UltralightView;");

        // Retrieve information about the Runnable class
        runtime.runnable.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("java/lang/Runnable")));
        runtime.runnable.run_method = env->GetMethodID(runtime.runnable.clazz, "run", "()V");

        // Retrieve information about the UltralightFileSystem class
        runtime.ultralight_file_system.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/plugin/filesystem/UltralightFileSystem")));
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
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/plugin/clipboard/UltralightClipboard")));
        runtime.ultralight_clipboard
            .clear_method = env->GetMethodID(runtime.ultralight_clipboard.clazz, "clear", "()V");
        runtime.ultralight_clipboard.read_plain_text_method =
            env->GetMethodID(runtime.ultralight_clipboard.clazz, "readPlainText", "()Ljava/lang/String;");
        runtime.ultralight_clipboard.write_plain_text_method =
            env->GetMethodID(runtime.ultralight_clipboard.clazz, "writePlainText", "(Ljava/lang/String;)V");

        // Retrieve information about the JavascriptContext class
        runtime.javascript_context.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptContext")));
        runtime.javascript_context.constructor = env->GetMethodID(
            runtime.javascript_context.clazz,
            "<init>",
            "(JLnet/janrupf/ultralight/javascript/JavascriptContextLock;)V");

        // Register native methods for the JavascriptContext class
        env->RegisterNatives(
            runtime.javascript_context.clazz,
            runtime.javascript_context.native_methods.data(),
            runtime.javascript_context.native_methods.size());

        // Retrieve information about the JavascriptContextLock
        runtime.javascript_context_lock.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptContextLock")));
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
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptGlobalContext")));
        runtime.javascript_global_context.constructor = env->GetMethodID(
            runtime.javascript_global_context.clazz,
            "<init>",
            "(JLnet/janrupf/ultralight/javascript/JavascriptContextLock;)V");

        // Register native methods for the JavascriptGlobalContext class
        env->RegisterNatives(
            runtime.javascript_global_context.clazz,
            runtime.javascript_global_context.native_methods.data(),
            runtime.javascript_global_context.native_methods.size());

        // Retrieve information about the JavascriptLockedObject interface
        runtime.javascript_locked_object.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptLockedObject")));
        runtime.javascript_locked_object.get_lock_method = env->GetMethodID(
            runtime.javascript_locked_object.clazz,
            "getLock",
            "()Lnet/janrupf/ultralight/javascript/JavascriptContextLock;");
        runtime.javascript_locked_object.get_context_handle_method =
            env->GetMethodID(runtime.javascript_locked_object.clazz, "getContextHandle", "()J");

        // Retrieve information about the JavascriptProtectedValue class
        runtime.javascript_protected_value.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptProtectedValue")));
        runtime.javascript_protected_value
            .constructor = env->GetMethodID(runtime.javascript_protected_value.clazz, "<init>", "(J)V");

        // Retrieve information about the JavascriptValue class
        runtime.javascript_value.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptValue")));
        runtime.javascript_value.constructor = env->GetMethodID(
            runtime.javascript_value.clazz, "<init>", "(JLnet/janrupf/ultralight/javascript/JavascriptContextLock;)V");

        // Register native methods for the JavascriptValue class
        env->RegisterNatives(
            runtime.javascript_value.clazz,
            runtime.javascript_value.native_methods.data(),
            runtime.javascript_value.native_methods.size());

        // Retrieve information about the JavascriptObject class
        runtime.javascript_object.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptObject")));
        runtime.javascript_object.constructor = env->GetMethodID(
            runtime.javascript_object.clazz, "<init>", "(JLnet/janrupf/ultralight/javascript/JavascriptContextLock;)V");

        // Register native methods for the JavascriptObject class
        env->RegisterNatives(
            runtime.javascript_object.clazz,
            runtime.javascript_object.native_methods.data(),
            runtime.javascript_object.native_methods.size());

        // Retrieve information about the JavascriptType enum
        runtime.javascript_type.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptType")));
        if(!runtime.javascript_type.constants.init(env, "net/janrupf/ultralight/javascript/JavascriptType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the JavascriptTypedArrayType enum
        runtime.javascript_typed_array_type.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptTypedArrayType")));
        if(!runtime.javascript_typed_array_type.constants
                .init(env, "net/janrupf/ultralight/javascript/JavascriptTypedArrayType")) {
            return JNI_EINVAL;
        }

        // Retrieve information about the JavascriptClass class
        runtime.javascript_class.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptClass")));
        runtime.javascript_class.constructor = env->GetMethodID(runtime.javascript_class.clazz, "<init>", "(J)V");

        // Register native methods for the JavascriptClass class
        env->RegisterNatives(
            runtime.javascript_class.clazz,
            runtime.javascript_class.native_methods.data(),
            runtime.javascript_class.native_methods.size());

        // Retrieve information about the JavascriptClassDefinition class
        runtime.javascript_class_definition.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptClassDefinition")));

        // Register native methods for the JavascriptClassDefinition class
        env->RegisterNatives(
            runtime.javascript_class_definition.clazz,
            runtime.javascript_class_definition.native_methods.data(),
            runtime.javascript_class_definition.native_methods.size());

        // Retrieve information about the JavascriptObjectConstructor interface
        runtime.javascript_object_constructor.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/interop/JavascriptObjectConstructor")));
        runtime.javascript_object_constructor.call_as_javascript_constructor_method = env->GetMethodID(
            runtime.javascript_object_constructor.clazz,
            "callAsJavascriptConstructor",
            "("
            "Lnet/janrupf/ultralight/javascript/JavascriptContext;"
            "Lnet/janrupf/ultralight/javascript/JavascriptObject;"
            "[Lnet/janrupf/ultralight/javascript/JavascriptValue;"
            ")Lnet/janrupf/ultralight/javascript/JavascriptObject;");

        // Retrieve information about the JavascriptObjectFinalizer interface
        runtime.javascript_object_finalizer.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/interop/JavascriptObjectFinalizer")));
        runtime.javascript_object_finalizer.finalize_javascript_object = env->GetMethodID(
            runtime.javascript_object_finalizer.clazz,
            "finalizeJavascriptObject",
            "(Lnet/janrupf/ultralight/javascript/JavascriptObject;)V");

        // Retrieve information about the JavascriptObjectFunction interface
        runtime.javascript_object_function.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/interop/JavascriptObjectFunction")));
        runtime.javascript_object_function.call_as_javascript_function = env->GetMethodID(
            runtime.javascript_object_function.clazz,
            "callAsJavascriptFunction",
            "("
            "Lnet/janrupf/ultralight/javascript/JavascriptContext;"
            "Lnet/janrupf/ultralight/javascript/JavascriptObject;"
            "Lnet/janrupf/ultralight/javascript/JavascriptObject;"
            "[Lnet/janrupf/ultralight/javascript/JavascriptValue;"
            ")Lnet/janrupf/ultralight/javascript/JavascriptValue;");

        // Retrieve information about the JavascriptObjectHasInstanceTester interface
        runtime.javascript_object_has_instance_tester.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("net/janrupf/ultralight/javascript/interop/JavascriptObjectHasInstanceTester")));
        runtime.javascript_object_has_instance_tester.has_javascript_instance_method = env->GetMethodID(
            runtime.javascript_object_has_instance_tester.clazz,
            "hasJavascriptInstance",
            "("
            "Lnet/janrupf/ultralight/javascript/JavascriptContext;"
            "Lnet/janrupf/ultralight/javascript/JavascriptObject;"
            "Lnet/janrupf/ultralight/javascript/JavascriptValue;"
            ")Z");

        // Retrieve information about the JavascriptObjectHasPropertyTester interface
        runtime.javascript_object_has_property_tester.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("net/janrupf/ultralight/javascript/interop/JavascriptObjectHasPropertyTester")));
        runtime.javascript_object_has_property_tester.has_javascript_property_method = env->GetMethodID(
            runtime.javascript_object_has_property_tester.clazz,
            "hasJavascriptProperty",
            "("
            "Lnet/janrupf/ultralight/javascript/JavascriptContext;"
            "Lnet/janrupf/ultralight/javascript/JavascriptObject;"
            "Ljava/lang/String;"
            ")Z");

        // Retrieve information about the JavascriptObjectInitializer interface
        runtime.javascript_object_initializer.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/interop/JavascriptObjectInitializer")));
        runtime.javascript_object_initializer.initialize_javascript_object_method = env->GetMethodID(
            runtime.javascript_object_initializer.clazz,
            "initializeJavascriptObject",
            "("
            "Lnet/janrupf/ultralight/javascript/JavascriptContext;"
            "Lnet/janrupf/ultralight/javascript/JavascriptObject;"
            ")V");

        // Retrieve information about the JavascriptObjectPropertyDeleter interface
        runtime.javascript_object_property_deleter.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("net/janrupf/ultralight/javascript/interop/JavascriptObjectPropertyDeleter")));
        runtime.javascript_object_property_deleter.delete_javascript_property_method = env->GetMethodID(
            runtime.javascript_object_property_deleter.clazz,
            "deleteJavascriptProperty",
            "("
            "Lnet/janrupf/ultralight/javascript/JavascriptContext;"
            "Lnet/janrupf/ultralight/javascript/JavascriptObject;"
            "Ljava/lang/String;"
            ")Z");

        // Retrieve information about the JavascriptObjectPropertyGetter interface
        runtime.javascript_object_property_getter.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("net/janrupf/ultralight/javascript/interop/JavascriptObjectPropertyGetter")));
        runtime.javascript_object_property_getter.get_javascript_property_method = env->GetMethodID(
            runtime.javascript_object_property_getter.clazz,
            "getJavascriptProperty",
            "("
            "Lnet/janrupf/ultralight/javascript/JavascriptContext;"
            "Lnet/janrupf/ultralight/javascript/JavascriptObject;"
            "Ljava/lang/String;"
            ")Lnet/janrupf/ultralight/javascript/JavascriptValue;");

        // Retrieve information about the JavascriptObjectPropertyNamesCollector interface
        runtime.javascript_object_property_names_collector.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("net/janrupf/ultralight/javascript/interop/JavascriptObjectPropertyNamesCollector")));
        runtime.javascript_object_property_names_collector.collect_javascript_property_names_method = env->GetMethodID(
            runtime.javascript_object_property_names_collector.clazz,
            "collectJavascriptPropertyNames",
            "("
            "Lnet/janrupf/ultralight/javascript/JavascriptContext;"
            "Lnet/janrupf/ultralight/javascript/JavascriptObject;"
            ")[Ljava/lang/String;");

        // Retrieve information about the JavascriptObjectPropertySetter interface
        runtime.javascript_object_property_setter.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("net/janrupf/ultralight/javascript/interop/JavascriptObjectPropertySetter")));
        runtime.javascript_object_property_setter.set_javascript_property_method = env->GetMethodID(
            runtime.javascript_object_property_setter.clazz,
            "setJavascriptProperty",
            "("
            "Lnet/janrupf/ultralight/javascript/JavascriptContext;"
            "Lnet/janrupf/ultralight/javascript/JavascriptObject;"
            "Ljava/lang/String;"
            "Lnet/janrupf/ultralight/javascript/JavascriptValue;"
            ")Z");

        // Retrieve information about the JavascriptObjectToTypeConverter interface
        runtime.javascript_object_to_type_converter.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(
            env->FindClass("net/janrupf/ultralight/javascript/interop/JavascriptObjectToTypeConverter")));
        runtime.javascript_object_to_type_converter.convert_to_javascript_type_method = env->GetMethodID(
            runtime.javascript_object_to_type_converter.clazz,
            "convertToJavascriptType",
            "("
            "Lnet/janrupf/ultralight/javascript/JavascriptContext;"
            "Lnet/janrupf/ultralight/javascript/JavascriptObject;"
            "Lnet/janrupf/ultralight/javascript/JavascriptType;"
            ")Lnet/janrupf/ultralight/javascript/JavascriptValue;");

        // Retrieve information about the Throwable class
        runtime.throwable.clazz = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("java/lang/Throwable")));
        runtime.throwable
            .get_message_method = env->GetMethodID(runtime.throwable.clazz, "getMessage", "()Ljava/lang/String;");

        // Retrieve information about the JavascriptException class
        runtime.javascript_exception.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptException")));
        runtime.javascript_exception.constructor = env->GetMethodID(
            runtime.javascript_exception.clazz,
            "<init>",
            "(Ljava/lang/String;Lnet/janrupf/ultralight/javascript/JavascriptValue;)V");

        // Retrieve information about the JavascriptEvaluationException class
        runtime.javascript_evaluation_exception.clazz = reinterpret_cast<jclass>(
            env->NewGlobalRef(env->FindClass("net/janrupf/ultralight/javascript/JavascriptEvaluationException")));

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
        env->DeleteGlobalRef(runtime.ultralight_load_listener.clazz);
        env->UnregisterNatives(runtime.ultralight_bitmap.clazz);
        env->DeleteGlobalRef(runtime.ultralight_bitmap.clazz);
        runtime.ultralight_bitmap_format.constants.clear(env);
        env->DeleteGlobalRef(runtime.ultralight_bitmap_format.clazz);
        env->DeleteGlobalRef(runtime.ultralight_bitmap_surface.clazz);
        env->UnregisterNatives(runtime.ultralight_surface.clazz);
        env->DeleteGlobalRef(runtime.ultralight_surface.clazz);
        env->DeleteGlobalRef(runtime.int_rect.clazz);
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
        env->DeleteGlobalRef(runtime.ultralight_config.clazz);
        env->UnregisterNatives(runtime.ultralight_platform.clazz);
        env->DeleteGlobalRef(runtime.ultralight_platform.clazz);
        env->DeleteGlobalRef(runtime.object_with_handle.clazz);
        env->DeleteGlobalRef(runtime.string.clazz);
    }
} // namespace ultralight_java