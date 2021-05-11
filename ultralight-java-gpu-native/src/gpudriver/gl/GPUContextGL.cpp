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

#include "ultralight_java/gpudriver/gl/GPUContextGL.h"

#include <ultralight_java/gpudriver/gl/glad.h>

#include "ultralight_java/gpudriver/gl/GPUDriverGL.h"

namespace ultralight {

    GPUContextGL::GPUContextGL(void *window, bool enable_msaa) : msaa_enabled_(enable_msaa) {

        window_ = window;
        active_window_ = window_;
        // TODO: enable msaa, GLFW currently has num_samples == 0 in glfwWindowHint
        int samples = 4;
        if(!samples) {
            msaa_enabled_ = false;
        }

        driver_.reset(new ultralight::GPUDriverGL(this));
    }

} // namespace ultralight