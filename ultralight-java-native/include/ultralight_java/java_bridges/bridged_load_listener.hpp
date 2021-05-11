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

#include <Ultralight/Ultralight.h>

#include "ultralight_java/util/jni_reference_wrapper.hpp"

namespace ultralight_java {
    /**
     * Bridge for com/labymedia/ultralight/plugin/loading/UltralightViewLoadListener to ultralight::LoadListener
     */
    class BridgedLoadListener : public ultralight::LoadListener, public JNIReferenceWrapper {
    public:
        /**
         * Constructs a new BridgedLoadListener wrapping an existing java listener.
         *
         * @param env The environment to use for wrapping the listener
         * @param listener The listener which should be wrapped
         */
        explicit BridgedLoadListener(JNIEnv *env, jobject listener);

        /**
         * Called when the view has begun loading.
         *
         * @param caller The view that has begun loading
         */
        void OnBeginLoading(
            ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url) final;

        /**
         * Called when the view has finished loading.
         *
         * @param caller The view that has finished loading
         */
        void OnFinishLoading(
            ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url) final;

        /**
         * Called when the view has failed to load.
         *
         * @param caller The view that has failed to load
         * @param url The url that failed to load
         * @param description A description of the error
         * @param error_domain The domain that failed to load
         * @param error_code The error code of the loading status
         */
        void OnFailLoading(
            ultralight::View *caller,
            uint64_t frame_id,
            bool is_main_frame,
            const ultralight::String &url,
            const ultralight::String &description,
            const ultralight::String &error_domain,
            int error_code) final;

        /**
         * Called when the view has updated the history.
         *
         * @param caller The view that has updated the history
         */
        void OnUpdateHistory(ultralight::View *caller) final;

        /**
         * Called when the window has been (re)created for the view.
         *
         * @param caller The view the window object has been recreated for
         */
        void OnWindowObjectReady(
            ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url) final;

        /**
         * Called when the DOM has been loaded for the view.
         *
         * @param caller The view the DOM has been loaded for
         */
        void OnDOMReady(
            ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url) final;
    };
} // namespace ultralight_java