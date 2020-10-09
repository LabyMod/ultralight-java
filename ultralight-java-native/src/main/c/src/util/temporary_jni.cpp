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

#include "ultralight_java/util/temporary_jni.hpp"

#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    TemporaryJNI::TemporaryJNI() {
        // Try to get a JNI environment
        int error = runtime.vm->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_8);
        if(error == JNI_EDETACHED) {
            // The current thread is detached, attach it and obtain the JNI environment
            // using the attach function
            runtime.vm->AttachCurrentThread(reinterpret_cast<void **>(&env), nullptr);
            detach_on_destruct = true;
        } else {
            detach_on_destruct = false;
        }
    }

    TemporaryJNI::~TemporaryJNI() {
        if(detach_on_destruct) {
            // The detach flag is set, detach the thread now
            runtime.vm->DetachCurrentThread();
        }
    }

    TemporaryJNI::operator JNIEnv *() {
        return env;
    }

    JNIEnv *TemporaryJNI::operator*() {
        return env;
    }

    JNIEnv *TemporaryJNI::operator->() {
        return env;
    }
}