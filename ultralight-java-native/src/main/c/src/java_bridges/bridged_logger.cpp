#include "ultralight_java/java_bridges/bridged_logger.hpp"

#include "ultralight_java/java_bridges/proxied_java_exception.hpp"
#include "ultralight_java/util/temporary_jni.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    BridgedLogger::BridgedLogger(JNIEnv *env, jobject logger) : JNIReferenceWrapper(env, logger) {}

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
}