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

#include <jni.h>
#include <Ultralight/Ultralight.h>

namespace ultralight_java {
    /**
     * Helper class for holding different javascript context locks.
     */
    class HoldJavascriptContextLock {
    private:
        enum Type {
            ULTRALIGHT,
            JNI
        };

        /**
         * Type of the hold lock, can be an ultralight one or a
         * JNI one.
         */
        Type type;

        union {
            /**
             * Ultralight variant of the lock. Only valid if type is Type::ULTRALIGHT!
             */
            ultralight::Ref<ultralight::JSContext> ultralight_lock;

            /**
             * JNI variant of the lock. Only valid if type is Type::JNI!
             */
            JSContextRef jni_lock;
        };

    public:
        /**
         * Constructs a new hold lock from an existing ultralight lock.
         *
         * @param lock The Ultralight lock to construct the hold lock from
         */
        explicit HoldJavascriptContextLock(ultralight::Ref<ultralight::JSContext> lock);

        /**
         * Constructs a new hold lock from an existing context ref.
         *
         * @param lock The context this lock holds
         */
        explicit HoldJavascriptContextLock(JSContextRef lock);

        /**
         * Destructs the hold lock and cleans up native resources if necessary.
         */
        ~HoldJavascriptContextLock();

        /**
         * Retrieves the locked context of this lock.
         *
         * @return The locked context
         */
        JSContextRef get_context();
    };

    /**
     * Class for interfacing with JSContextRef from java.
     */
    class JavascriptContextLockJNI {
    public:
        /**
         * Converts the given Ultralight JS lock into a java object.
         *
         * @param env The JNI environment to use for accessing java
         * @param lock The lock to convert
         * @return The converted lock
         */
        static jobject create(JNIEnv *env, ultralight::Ref<ultralight::JSContext> lock);

        /**
         * Converts the given JSContextRef into a java object.
         *
         * @param env The JNI environment to use for accessing java
         * @param context The context to convert
         * @return The converted context
         */
        static jobject create(JNIEnv *env, JSContextRef context);

        /**
         * Retrieves the locked context.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context lock
         * @return The locked context as a java object
         */
        static jobject get_context(JNIEnv *env, jobject java_instance);

        /**
         * Releases (unlocks) the lock.
         *
         * @param env The JNI environment to use for accessing java
         * @param java_instance The java instance of this context lock
         */
        static void release(JNIEnv *env, jobject java_instance);
    };
}