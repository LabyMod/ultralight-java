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

#include "ultralight_java/java_bridges/ultralight_ref_ptr_jni.hpp"

namespace ultralight_java {
    void UltralightRefPtrJNI::_delete(JNIEnv *, jclass, jlong handle) {
        // Get back the wrapper
        auto *ptr = reinterpret_cast<WrappedRefPtr *>(handle);

        // Invoke the deleter to delete the wrapped pointer and
        // then delete the wrapper itself
        ptr->deleter(ptr->ptr_value);
        delete ptr;
    }
}