#pragma once

#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/Logger.h>
#include <array>
#include <jni.h>

#include "ultralight_java/util/java_enum.hpp"

namespace ultralight_java {
    class BridgedLogger;
    class BridgedFileSystem;

    struct UltralightJavaRuntime {
        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Method returning the native pointer of the underlying C(++) object
             */
            jmethodID get_handle_method;
        } object_with_handle;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a long as a handle
             */
            jmethodID constructor;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 7> native_methods;
        } ultralight_platform;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /* String */ jfieldID resource_path_field;
            /* String */ jfieldID cache_path_field;
            /* boolean */ jfieldID use_gpu_renderer_field;
            /* double */ jfieldID device_scale_field;
            /* FaceWinding */ jfieldID face_winding_field;
            /* boolean */ jfieldID enable_images_field;
            /* boolean */ jfieldID enable_javascript_field;
            /* FontHinting */ jfieldID font_hinting_field;
            /* double */ jfieldID font_gamma_field;
            /* String */ jfieldID font_family_standard_field;
            /* String */ jfieldID font_family_fixed_field;
            /* String */ jfieldID font_family_serif_field;
            /* String */ jfieldID font_family_sans_serif_field;
            /* String */ jfieldID user_agent_field;
            /* String */ jfieldID user_stylesheet_field;
            /* boolean */ jfieldID force_repaint_field;
            /* double */ jfieldID animation_timer_delay_field;
            /* double */ jfieldID scroll_timer_delay_field;
            /* double */ jfieldID recycle_delay_field;
            /* long */ jfieldID memory_cache_size_field;
            /* long */ jfieldID page_cache_size_field;
            /* long */ jfieldID override_ram_size_field;
            /* long */ jfieldID min_large_heap_size_field;
            /* long */ jfieldID min_small_heap_size_field;
        } ultralight_config;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The enum constants
             */
            JavaEnum<ultralight::FaceWinding> constants;
        } face_winding;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The java enum constants
             */
            JavaEnum<ultralight::FontHinting> constants;
        } font_hinting;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /*
             * Method taking a log level and message to log
             */
            jmethodID log_message_method;
        } logger;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The java enum constants
             */
            JavaEnum<ultralight::LogLevel> constants;
        } log_level;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a long as a handle
             */
            jmethodID constructor;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 1> native_methods;
        } ref_ptr;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a java RefPtr as handle
             */
            jmethodID constructor;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 6> native_methods;
        } ultralight_renderer;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a java RefPtr as handle
             */
            jmethodID constructor;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 29> native_methods;
        } ultralight_view;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking 4 ints as the bounds in coordinates
             */
            jmethodID bounds_constructor;

            /* int */ jfieldID left_field;
            /* int */ jfieldID top_field;
            /* int */ jfieldID right_field;
            /* int */ jfieldID bottom_field;
        } int_rect;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a UltralightView and a long as a handle
             */
            jmethodID constructor;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 10> native_methods;
        } ultralight_surface;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a UltralightView and a long as a handle
             */
            jmethodID constructor;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 1> native_methods;
        } ultralight_bitmap_surface;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The java enum constants
             */
            JavaEnum<ultralight::BitmapFormat> constants;
        } ultralight_bitmap_format;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a RefPtr as handle
             */
            jmethodID constructor;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 22> native_methods;
        } ultralight_bitmap;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID on_begin_loading_method;
            jmethodID on_finish_loading_method;
            jmethodID on_fail_loading_method;
            jmethodID on_update_history_method;
            jmethodID on_window_object_ready_method;
            jmethodID on_dom_ready_method;
        } ultralight_load_listener;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /* int */ jfieldID id_field;
        } ultralight_key;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /* UltralightKeyEventType */ jfieldID type_field;
            /* int */ jfieldID modifiers_field;
            /* UltralightKey */ jfieldID virtual_key_code_field;
            /* int */ jfieldID native_key_code_field;
            /* String */ jfieldID key_identifier_field;
            /* String */ jfieldID text_field;
            /* String */ jfieldID unmodified_text_field;
            /* boolean */ jfieldID is_keypad_field;
            /* boolean */ jfieldID is_auto_repeat_field;
            /* boolean */ jfieldID is_system_key_field;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 1> native_methods;
        } ultralight_key_event;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The java enum constants
             */
            JavaEnum<ultralight::KeyEvent::Type> constants;
        } ultralight_key_event_type;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /* UltralightMouseEventType */ jfieldID type_field;
            /* int */ jfieldID x_field;
            /* int */ jfieldID y_field;
            /* UltralightMouseButton */ jfieldID button_field;
        } ultralight_mouse_event;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The java enum constants
             */
            JavaEnum<ultralight::MouseEvent::Button> constants;
        } ultralight_mouse_event_button;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The java enum constants
             */
            JavaEnum<ultralight::MouseEvent::Type> constants;
        } ultralight_mouse_event_type;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /* UltralightScrollEventType */ jfieldID type_field;
            /* int */ jfieldID delta_x_field;
            /* int */ jfieldID delta_y_field;
        } ultralight_scroll_event;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The java enum constants
             */
            JavaEnum<ultralight::ScrollEvent::Type> constants;
        } ultralight_scroll_event_type;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * THe java enum constants
             */
            JavaEnum<ultralight::Cursor> constants;
        } ultralight_cursor;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The java enum constants
             */
            JavaEnum<ultralight::MessageLevel> constants;
        } message_level;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The java enum constants
             */
            JavaEnum<ultralight::MessageSource> constants;
        } message_source;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID on_change_title_method;
            jmethodID on_change_url_method;
            jmethodID on_change_tooltip_method;
            jmethodID on_change_cursor_method;
            jmethodID on_add_console_message_method;
            jmethodID on_create_child_view_method;
        } ultralight_view_listener;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID run_method;
        } runnable;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID file_exists_method;
            jmethodID get_file_size_method;
            jmethodID get_file_mime_type_method;
            jmethodID open_file_method;
            jmethodID close_file_method;
            jmethodID read_from_file_method;
        } ultralight_file_system;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;
        } javascript_exception;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;
        } null_pointer_exception;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;
        } illegal_state_exception;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;
        } illegal_argument_exception;

        /**
         * The JavaVM instance executing the virtual machine.
         */
        JavaVM *vm;

        /**
         * Current global logger instance, or nullptr, if none
         */
        BridgedLogger *bridged_logger;

        /**
         * Current global file system instance, or nullptr, if none
         */
        BridgedFileSystem *bridged_file_system;
    };

    /**
     * Global runtime struct for easy access of constants.
     */
    extern UltralightJavaRuntime runtime;

    /**
     * Initializes the library using the JavaVM instance.
     *
     * @param vm The currently executing JavaVM
     * @return
     */
    int setup(JavaVM *vm);

    /**
     * Frees global references.
     *
     * @param vm The currently executing JavaVM
     */
    void destruct(JavaVM *vm);
} // namespace ultralight_java