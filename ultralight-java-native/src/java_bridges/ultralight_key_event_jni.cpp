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