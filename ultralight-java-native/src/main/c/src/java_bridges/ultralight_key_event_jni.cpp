#include "ultralight_java/java_bridges/ultralight_key_event_jni.hpp"

#include "ultralight_java/ultralight_java_instance.hpp"
#include "ultralight_java/util/util.hpp"

namespace ultralight_java {
    jstring UltralightKeyEventJNI::get_key_identifier_from_virtual_key_code(JNIEnv *env, jclass caller_class,
                                                                            jobject virtual_key_code) {
        ultralight::String identifier;

        if (!virtual_key_code) {
            ultralight::GetKeyIdentifierFromVirtualKeyCode(0, identifier);
        } else {
            ultralight::GetKeyIdentifierFromVirtualKeyCode(
                    env->GetIntField(virtual_key_code, runtime.ultralight_key.id_field), identifier);
        }

        return Util::create_jstring_from_utf16(env, identifier.utf16());
    }
}