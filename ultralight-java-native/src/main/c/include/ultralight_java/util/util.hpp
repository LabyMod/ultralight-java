#pragma once

#include <jni.h>
#include <Ultralight/Ultralight.h>

#define JNI_STRING16_OR_NPE(x, env, str, msg) \
    ([&]() -> bool { \
        if(!str) { \
            env->ThrowNew(::ultralight_java::runtime.null_pointer_exception.clazz, msg); \
            return false; \
        } \
        (x) = ::ultralight_java::Util::create_utf16_from_jstring(env, reinterpret_cast<jstring>(str)); \
        return true; \
    })()

namespace ultralight_java {
    /**
     * Static utility class for performing various utility functions
     */
    class Util {
    public:
        // Static utility class
        explicit Util() = delete;

        /**
         * Creates an ultralight::String16 from a java string.
         *
         * @param env The JNI environment to use for accessing java
         * @param str The java string to convert
         * @return The converted string
         */
        static ultralight::String16 create_utf16_from_jstring(JNIEnv *env, jstring str);

        /**
         * Creates a jstring from an ultralight::String16.
         *
         * @param env The JNI environment to use for accessing java
         * @param str The Ultralight string to convert
         * @return The converted string
         */
        static jstring create_jstring_from_utf16(JNIEnv *env, const ultralight::String16 &str);

        /**
         * Creates an ultralight::IntRect from a java IntRect.
         *
         * @param env The JNI environment to use for accessing java
         * @param object The object to convert
         * @return The converted int rect
         */
        static ultralight::IntRect create_int_rect_from_jobject(JNIEnv *env, jobject object);

        /**
         * Creates a jobject from an ultralight::IntRect.
         *
         * @param env The JNI environment to use for accessing java
         * @param int_rect The rectangle to convert
         * @return The created jobject
         */
        static jobject create_jobject_from_int_rect(JNIEnv *env, const ultralight::IntRect &int_rect);

        /**
         * Creates an ultralight::BitmapFormat from a java UltralightBitmapFormat.
         *
         * @param env The JNI environment to use for accessing java
         * @param format The java format to convert
         * @return The converted bitmap format
         */
        static ultralight::BitmapFormat create_bitmap_format_from_jobject(JNIEnv *env, jobject format);

        /**
         * Creates a jobject from a ultralight::BitmapFormat.
         *
         * @param env The JNI environment to use for accessing java
         * @param format The native format to convert
         * @return The converted jobject
         */
        static jobject create_jobject_from_bitmap_format(JNIEnv *env, const ultralight::BitmapFormat &format);

        /**
         * Creates an ultralight::KeyEvent::Type from a java UltralightKeyEventType.
         *
         * @param env The JNI environment to use for accessing java
         * @param type The java type to convert
         * @return The converted type
         */
        static ultralight::KeyEvent::Type create_key_event_type_from_jobject(JNIEnv *env, jobject type);

        /**
         * Converts a java object to a native ultralight::KeyEvent.
         *
         * @param env The JNI environment to use for accessing java
         * @param event The java object of the event to convert
         * @return The converted event
         */
        static ultralight::KeyEvent create_key_event_from_jobject(JNIEnv *env, jobject event);

        /**
         * Creates an ultralight::MouseEvent::Type from a java UltralightMouseEventType.
         *
         * @param env The JNI environment to use for accessing java
         * @param type The java type to convert
         * @return The converted type
         */
        static ultralight::MouseEvent::Type create_mouse_event_type_from_jobject(JNIEnv *env, jobject type);

        /**
         * Creates an ultralight::MouseEvent::Button from a java UltralightMouseEventButton.
         *
         * @param env The JNI environment to use for accessing java
         * @param button The java button to convert
         * @return The converted button
         */
        static ultralight::MouseEvent::Button create_mouse_event_button_from_jobject(JNIEnv *env, jobject button);

        /**
         * Converts a java object to a native ultralight::MouseEvent.
         *
         * @param env The JNI environment to use for accessing java
         * @param event The java object of the event to convert
         * @return The converted event
         */
        static ultralight::MouseEvent create_mouse_event_from_jobject(JNIEnv *env, jobject event);

        /**
         * Creates an ultralight::ScrollEvent::Type from a java UltralightScrollEventType.
         *
         * @param env The JNI environment to use for accessing java
         * @param button The java type to convert
         * @return The converted type
         */
        static ultralight::ScrollEvent::Type create_scroll_event_type_from_jobject(JNIEnv *env, jobject event);

        /**
         * Converts a java object to a native ultralight::ScrollEvent.
         *
         * @param env The JNI environment to use for accessing java
         * @param event The java object of the event to convert
         * @return The converted event
         */
        static ultralight::ScrollEvent create_scroll_event_from_jobject(JNIEnv *env, jobject event);
    };
}