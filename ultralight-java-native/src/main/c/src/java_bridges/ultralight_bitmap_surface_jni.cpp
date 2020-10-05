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

#include "ultralight_java/java_bridges/ultralight_bitmap_surface_jni.hpp"

#include "ultralight_java/java_bridges/ultralight_ref_ptr_jni.hpp"
#include "ultralight_java/ultralight_java_instance.hpp"

namespace ultralight_java {
    jobject UltralightBitmapSurfaceJNI::bitmap(JNIEnv *env, jobject instance) {
        auto *surface = reinterpret_cast<ultralight::BitmapSurface *>(
                env->CallLongMethod(instance, runtime.object_with_handle.get_handle_method));
        if (env->ExceptionCheck()) {
            return nullptr;
        }

        auto bitmap = surface->bitmap();
        return env->NewObject(runtime.ultralight_bitmap.clazz, runtime.ultralight_bitmap.constructor,
                              UltralightRefPtrJNI::create<ultralight::Bitmap>(env, std::move(
                                      ultralight::RefPtr<ultralight::Bitmap>(std::move(bitmap)))));
    }
}