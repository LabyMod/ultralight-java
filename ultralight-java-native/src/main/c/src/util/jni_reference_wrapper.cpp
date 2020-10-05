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

#include "ultralight_java/util/jni_reference_wrapper.hpp"

#include "ultralight_java/util/temporary_jni.hpp"

namespace ultralight_java {
    JNIReferenceWrapper::JNIReferenceWrapper(JNIReferenceWrapper &&other) noexcept: reference(other.reference) {
        other.reference = nullptr;
    }

    JNIReferenceWrapper::JNIReferenceWrapper(JNIEnv *env, jobject reference) : reference(
            env->NewGlobalRef(reference)) {}

    JNIReferenceWrapper::~JNIReferenceWrapper() {
        if (reference) {
            TemporaryJNI jni;
            jni->DeleteGlobalRef(reference);
        }
    }

    JNIReferenceWrapper::operator jobject() {
        return reference;
    }

    jobject JNIReferenceWrapper::operator*() {
        return reference;
    }

    jobject JNIReferenceWrapper::get() {
        return reference;
    }
}