/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 LabyMedia and contributors
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
     * Bridge for net/labymedia/ultralight/plugin/view/UltralightViewListener to ultralight::ViewListener
     */
    class BridgedViewListener : public ultralight::ViewListener, public JNIReferenceWrapper {
    public:
        /**
         * Constructs a new BridgedViewListener wrapping an existing java listener.
         *
         * @param env The environment to use for wrapping the listener
         * @param listener The listener which should be wrapped
         */
        explicit BridgedViewListener(JNIEnv *env, jobject listener);

        /**
         * Called when the page title changes.
         *
         * @param caller The view the title has changed for
         * @param title The new title
         */
        void OnChangeTitle(ultralight::View *caller, const ultralight::String &title) final;

        /**
         * Called when the page URL changes.
         *
         * @param caller The view the url has changed for
         * @param url The new url
         */
        void OnChangeURL(ultralight::View *caller, const ultralight::String &url) final;

        /**
         * Called when the tooltip changes (usually as result of a mouse hover).
         *
         * @param caller The view the tooltip has changed for
         * @param tooltip The new tooltip
         */
        void OnChangeTooltip(ultralight::View *caller, const ultralight::String &tooltip) final;

        /**
         * Called when the mouse cursor changes.
         *
         * @param caller The view the cursor has changed for
         * @param cursor The new cursor
         */
        void OnChangeCursor(ultralight::View *caller, ultralight::Cursor cursor) final;

        /**
         * Called when a message is added to the console (useful for errors / debug).
         *
         * @param caller The view that has emitted the message
         * @param source The source the message originated from
         * @param level The log level of the message
         * @param message The message itself
         * @param line_number The line the message was emitted from
         * @param column_number The column the message was emitted from
         * @param source_id The id of the source the message was emitted from
         */
        void OnAddConsoleMessage(
            ultralight::View *caller,
            ultralight::MessageSource source,
            ultralight::MessageLevel level,
            const ultralight::String &message,
            uint32_t line_number,
            uint32_t column_number,
            const ultralight::String &source_id) final;

        /**
         * Called when the page wants to create a new View.
         *
         * @param caller The view that has requested to create a new View
         * @param opener_url The url of the view requesting to open a new View
         * @param target_url The url the new view should contain
         * @param is_popup Whether the new view is a popup
         * @param popup_rect The bounding rectangle of the new view, might be invalid
         * @return The created view, or nullptr, if the request should be ignored
         */
        ultralight::RefPtr<ultralight::View> OnCreateChildView(
            ultralight::View *caller,
            const ultralight::String &opener_url,
            const ultralight::String &target_url,
            bool is_popup,
            const ultralight::IntRect &popup_rect) final;
    };
} // namespace ultralight_java