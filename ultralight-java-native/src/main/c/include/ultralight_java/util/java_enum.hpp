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
#include <string>
#include <vector>
#include <unordered_map>

namespace ultralight_java {
    class UltralightJavaRuntime;
    extern UltralightJavaRuntime runtime;

    /**
     * Class for simplifying the bridging of java enums
     */
    template<typename t_Native>
    class JavaEnum {
    private:
        std::unordered_map<jobject, t_Native> java_to_native_values;
        std::unordered_map<t_Native, jobject> native_to_java_values;
        std::unordered_map<t_Native, std::string> native_to_name_values;

        UltralightJavaRuntime *_runtime();

        /**
         * Indexes a value and adds it to the map for deferred initialization.
         *
         * @tparam t_Key The type of the key, needs to be compatible with t_Native
         * @tparam t_Value The type of the value, needs to be compatible with t_Value
         * @tparam t_Other The other argument types
         * @param key The key to index
         * @param value The value mapped to the key
         * @param other The other arguments, recursively passed to index() again
         */
        template<typename t_Key, typename t_Value, typename ...t_Other>
        inline void index(t_Key &&key, t_Value &&value, t_Other &&...other) {
            native_to_name_values.insert(std::make_pair<t_Native, std::string>(static_cast<t_Native &&>(key),
                                                                               static_cast<std::string &&>(value)));
            index(std::forward<t_Other>(other)...);
        }

        /**
         * Stub method to allow the sequence of arguments to end
         */
        inline void index() {}

    public:
        /**
         * Constructs a new JavaEnum mappings enum constants from java.
         *
         * @tparam t_Args The types of the arguments
         * @param args The argument to use for mapping, always [key, value] pairs split across arguments
         */
        template<typename ...t_Args>
        explicit JavaEnum(t_Args &&...args) {
            static_assert(sizeof...(args) % 2 == 0, "Size of arguments needs to be even");
            index(std::forward<t_Args>(args)...);
        }

        /**
         * Initializes this java enum.
         *
         * @param env The JNI environment to use for accessing java
         * @param class_name The binary name of the java class
         * @return Whether initialization succeeded
         */
        bool init(JNIEnv *env, const std::string &class_name) {
            // Find the class
            jclass clazz = env->FindClass(class_name.c_str());
            if (!clazz) {
                // Class not found
                return false;
            }

            // Construct the required type signatures
            std::string class_signature = "L" + class_name + ";";
            std::string array_class_signature = "[" + class_signature;

            // Find the required methods
            jmethodID value_of_method =
                    env->GetStaticMethodID(clazz, "valueOf", ("(Ljava/lang/String;)" + class_signature).c_str());
            if(!value_of_method) {
                return false;
            }

            // Iterate over all known constants
            for (const auto &[native, name] : native_to_name_values) {
                // Invoke the Enum#valueOf(String) method to get a reference to the
                // named element
                jstring name_str = env->NewStringUTF(name.c_str());
                jobject value = env->CallStaticObjectMethod(clazz, value_of_method, name_str);
                env->DeleteLocalRef(name_str);

                if(env->ExceptionCheck()) {
                    // An exception occurred, the value is invalid
                    env->DeleteLocalRef(clazz);
                    return false;
                }

                // Get a global ref and delete the local one
                jobject g_ref = env->NewGlobalRef(value);
                env->DeleteLocalRef(value);

                // Save the value and its name
                java_to_native_values.insert(std::make_pair(g_ref, native));
                native_to_java_values.insert(std::make_pair(native, g_ref));
            }

            return true;
        }

        /**
         * Deletes all references this java enum has created.
         *
         * @param env The JNI environment to use for accessing java
         */
        void clear(JNIEnv *env) {
            for(auto &[g_ref, _] : java_to_native_values) {
                env->DeleteGlobalRef(g_ref);
            }
        }

        /**
         * Converts a java enum value to its native value.
         *
         * @param env The JNI environment to use for accessing java
         * @param instance The java value of the enum
         * @param out A pointer to write the native value to
         * @return Whether the conversion succeeded
         */
        bool from_java(JNIEnv *env, jobject instance, t_Native *out) {
            if(!instance) {
                env->ThrowNew(_runtime()->null_pointer_exception.clazz, "Can't convert null constant to native enum");
                return false;
            }

            for(auto &[g_ref, native] : java_to_native_values) {
                if(env->IsSameObject(g_ref, instance)) {
                    *out = native;
                    return true;
                }
            }

            env->ThrowNew(_runtime()->illegal_argument_exception.clazz, "Invalid java enum constant passed in");
            return false;
        }

        /**
         * Converts a native value to its java value.
         *
         * @param env The JNI environment to use for accessing java
         * @param native The native value
         * @return The created java enum constant, or nullptr, if the conversion failed
         */
        jobject to_java(JNIEnv *env, t_Native native) {
            if(auto it = native_to_java_values.find(native); it != native_to_java_values.end()) {
                return env->NewLocalRef(it->second);
            }

            env->ThrowNew(_runtime()->illegal_argument_exception.clazz, "Invalid native enum constant passed in");
            return nullptr;
        }
    };

    template <typename t_Native>
    UltralightJavaRuntime *JavaEnum<t_Native>::_runtime() {
        return &runtime;
    }
}