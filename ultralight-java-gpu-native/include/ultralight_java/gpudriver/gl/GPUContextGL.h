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

#pragma once
#include <Ultralight/platform/Config.h>
#include <Ultralight/platform/GPUDriver.h>
#include <memory>

#include "ultralight_java/gpudriver/common/GPUDriverImpl.h"

typedef struct GLFWwindow GLFWwindow;

#define ENABLE_OFFSCREEN_GL 0

namespace ultralight {

    class GPUContextGL {
    protected:
        std::unique_ptr<ultralight::GPUDriverImpl> driver_;
        void *window_;
        void *active_window_ = nullptr;
        bool msaa_enabled_;

    public:
        GPUContextGL(void *window, bool enable_msaa);

        virtual ~GPUContextGL() {
        }

        virtual ultralight::GPUDriverImpl *driver() const {
            return driver_.get();
        }

        virtual ultralight::FaceWinding face_winding() const {
            return ultralight::kFaceWinding_CounterClockwise;
        }

        virtual void BeginDrawing() {
        }

        virtual void EndDrawing() {
        }

        virtual bool msaa_enabled() const {
            return msaa_enabled_;
        }

        // An offscreen window dedicated to maintaining the OpenGL context.
        // All other windows created during lifetime of the app share this context.
        virtual void *window() {
            return window_;
        }

        // FBOs are not shared across contexts in OpenGL 3.2 (AFAIK), we luckily
        // don't need to share them across multiple windows anyways so we temporarily
        // set the active GL context to the "active window" when creating FBOs.
        virtual void set_active_window(void *win) {
            active_window_ = win;
        }

        virtual void *active_window() {
            return active_window_;
        }
    };

} // namespace ultralight