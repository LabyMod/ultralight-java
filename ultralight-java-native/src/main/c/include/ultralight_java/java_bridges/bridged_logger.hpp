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