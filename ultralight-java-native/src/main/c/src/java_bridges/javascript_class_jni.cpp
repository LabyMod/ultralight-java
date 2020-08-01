#include "ultralight_java/java_bridges/javascript_class_jni.hpp"

#include "JavaScriptCore/JavaScript.h"

namespace ultralight_java {
    void JavascriptClassJNI::release(JNIEnv *, jclass, jlong handle) {
        JSClassRelease(reinterpret_cast<JSClassRef>(handle));
    }
}