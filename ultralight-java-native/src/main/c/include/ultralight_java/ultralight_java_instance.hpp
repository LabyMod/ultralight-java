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

#pragma once

#include <Ultralight/Ultralight.h>
#include <Ultralight/platform/Logger.h>
#include <array>
#include <jni.h>

#include "ultralight_java/util/java_enum.hpp"

namespace ultralight_java {
    class BridgedLogger;
    class BridgedFileSystem;
    class BridgedClipboard;

    struct UltralightJavaRuntime {
        struct {
            /**
             * The java class itself
             */
            jclass clazz;
        } string;

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
            std::array<JNINativeMethod, 30> native_methods;
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

            jmethodID clear_method;
            jmethodID read_plain_text_method;
            jmethodID write_plain_text_method;
        } ultralight_clipboard;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a long as handle and a lock
             */
            jmethodID constructor;

            /**
             * Natives method that should be bound
             */
            std::array<JNINativeMethod, 16> native_methods;
        } javascript_context;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a long as handle
             */
            jmethodID constructor;

            jmethodID unlock_method;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 2> native_methods;
        } javascript_context_lock;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a long as handle and a lock
             */
            jmethodID constructor;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 3> native_methods;
        } javascript_global_context;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Method returning a java object representing the lock
             */
            jmethodID get_lock_method;

            /**
             * Method returning a C++ pointer to the underlying context object
             */
            jmethodID get_context_handle_method;

            /**
             * Method returning a C++ pointer to the underlying lock object.
             */
            jmethodID get_lock_handle_method;
        } javascript_locked_object;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a long as handle
             */
            jmethodID constructor;
        } javascript_protected_value;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a long as handle and a lock
             */
            jmethodID constructor;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 21> native_methods;
        } javascript_value;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a long as handle and a lock
             */
            jmethodID constructor;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 19> native_methods;
        } javascript_object;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The java enum constants
             */
            JavaEnum<JSType> constants;
        } javascript_type;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * The java enum constants
             */
            JavaEnum<JSTypedArrayType> constants;
        } javascript_typed_array_type;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking long as a handle
             */
            jmethodID constructor;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 1> native_methods;
        } javascript_class;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Native methods that should be bound
             */
            std::array<JNINativeMethod, 19> native_methods;
        } javascript_class_definition;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID call_as_javascript_constructor_method;
        } javascript_object_constructor;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID finalize_javascript_object;
        } javascript_object_finalizer;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID call_as_javascript_function;
        } javascript_object_function;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID has_javascript_instance_method;
        } javascript_object_has_instance_tester;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID has_javascript_property_method;
        } javascript_object_has_property_tester;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID initialize_javascript_object_method;
        } javascript_object_initializer;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID delete_javascript_property_method;
        } javascript_object_property_deleter;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID get_javascript_property_method;
        } javascript_object_property_getter;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID collect_javascript_property_names_method;
        } javascript_object_property_names_collector;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID set_javascript_property_method;
        } javascript_object_property_setter;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID convert_to_javascript_type_method;
        } javascript_object_to_type_converter;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            jmethodID get_message_method;
        } throwable;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;

            /**
             * Constructor taking a string as a message and a Javascript value as
             * the original exception
             */
            jmethodID constructor;
        } javascript_exception;

        struct {
            /**
             * The java class itself
             */
            jclass clazz;
        } javascript_evaluation_exception;

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

        /**
         * Current global clipboard instance, or nullptr, if none
         */
        BridgedClipboard *bridged_clipboard;
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