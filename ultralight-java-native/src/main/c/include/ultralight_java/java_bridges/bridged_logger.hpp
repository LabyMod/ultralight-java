/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <Ultralight/platform/Logger.h>
#include "ultralight_java/util/jni_reference_wrapper.hpp"

namespace ultralight_java {
    /**
     * Bridge for net/labymedia/ultralight/plugin/logging/Logger to ultralight::Logger
     */
    class BridgedLogger : public ultralight::Logger, public JNIReferenceWrapper {
    public:
        /**
         * Creates a new BridgedLogger using a JNI environment and a java instance.
         *
         * @param env The environment to use for referencing the logger
         * @param logger The java instance of the logger
         */
        explicit BridgedLogger(JNIEnv *env, jobject logger);

        /**
         * Called when the library wants to print a message to the log.
         *
         * @param log_level The level of the message that should be printed
         * @param message The message that should be printed
         */
        void LogMessage(ultralight::LogLevel log_level, const ultralight::String16 &message) final;
    };
}