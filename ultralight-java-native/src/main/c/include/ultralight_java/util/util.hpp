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

#pragma once

#include <Ultralight/Ultralight.h>
#include <jni.h>
#include <string>

#define JNI_STRING16_OR_NPE(x, env, str, msg)                                                                          \
    ([&]() -> bool {                                                                                                   \
        if(!str) {                                                                                                     \
            env->ThrowNew(::ultralight_java::runtime.null_pointer_exception.clazz, msg);                               \
            return false;                                                                                              \
        }                                                                                                              \
        (x) = ::ultralight_java::Util::create_utf16_from_jstring(env, reinterpret_cast<jstring>(str));                 \
        return true;                                                                                                   \
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
          * Creates a std::string from a javascript string.
          *
          * @param str The string to convert
          * @return The converted string
          */
        static std::string create_utf8_from_jsstring_ref(JSStringRef str);

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
         * Converts a java object to a native ultralight::KeyEvent.
         *
         * @param env The JNI environment to use for accessing java
         * @param event The java object of the event to convert
         * @return The converted event
         */
        static ultralight::KeyEvent create_key_event_from_jobject(JNIEnv *env, jobject event);

        /**
         * Converts a java object to a native ultralight::MouseEvent.
         *
         * @param env The JNI environment to use for accessing java
         * @param event The java object of the event to convert
         * @return The converted event
         */
        static ultralight::MouseEvent create_mouse_event_from_jobject(JNIEnv *env, jobject event);

        /**
         * Converts a java object to a native ultralight::ScrollEvent.
         *
         * @param env The JNI environment to use for accessing java
         * @param event The java object of the event to convert
         * @return The converted event
         */
        static ultralight::ScrollEvent create_scroll_event_from_jobject(JNIEnv *env, jobject event);

        /**
         * Converts a javascript string to a java string.
         *
         * @param env The JNI environment to use for accessing java
         * @param javascript_string The javascript string to convert
         * @return The converted string as a java string
         */
        static jstring create_jstring_from_jsstring_ref(JNIEnv *env, JSStringRef javascript_string);

        /**
         * Converts a java string to a javascript string.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_string The java string to convert
         * @return The converted string as a javascript string
         */
        static JSStringRef create_jsstring_ref_from_jstring(JNIEnv *env, jstring java_string);

        /**
         * Throws a javascript value as a java exception. This is meant to be used to translate javascript exceptions
         * to java exceptions.
         *
         * @param message The message to pass to the java exception
         * @param context The javascript context to use for accessing javascript
         * @param javascript_value The javascript value to throw
         * @param env The JNI environment to use for accessing java
         * @param lock The java representation of the context lock
         */
        static void throw_jssvalue_ref_as_java_exception(
            const std::string& message, JSContextRef context, JSValueRef javascript_value, JNIEnv *env, jobject lock);

        /**
         * Creates a javascript value from a java exception.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_throwable The throwable to convert
         * @param context The javascript context to use for accessing javascript
         * @return The converted throwable
         */
        static JSValueRef create_jssvalue_from_jthrowable(JNIEnv *env, jthrowable java_throwable, JSContextRef context);

        /**
         * Creates a javascript exception with the specified message.
         *
         * @param context The javascript context to use for accessing javascript
         * @param message The message of the exception
         * @return The created exception
         */
        static JSValueRef create_jserror(JSContextRef context, const std::string& message);

        /**
         * Translates an array of JavascriptValue objects to JSValueRef's.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_arguments The array of arguments to translate
         * @return The translated array, or nullptr, if the argument length is 0
         */
        static JSValueRef *translate_bridged_arguments(JNIEnv *env, jobjectArray java_arguments);
    };
} // namespace ultralight_java