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

#include "ultralight_java/java_bridges/bridged_logger.hpp"

#include "ultralight_java/java_bridges/proxied_java_exception.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/temporary_jni.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    BridgedLogger::BridgedLogger(JNIEnv *env, jobject logger) : JNIReferenceWrapper(env, logger) {
    }

    void BridgedLogger::LogMessage(ultralight::LogLevel log_level, const ultralight::String16 &message) {
        TemporaryJNI env;

        // Convert the level
        jobject java_log_level = runtime.log_level.constants.to_java(env, log_level);

        // Convert the message
        jstring java_message = Util::create_jstring_from_utf16(env, message);

        // Invoke the logMessage method
        env->CallVoidMethod(reference, runtime.logger.log_message_method, java_log_level, java_message);

        // Clean up the references
        env->DeleteLocalRef(java_message);
        env->DeleteLocalRef(java_log_level);

        // Possibly throw exception if one occurred
        ProxiedJavaException::throw_if_any(env);
    }
} // namespace ultralight_java