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

#include <Ultralight/platform/Clipboard.h>

#include "ultralight_java/util/jni_reference_wrapper.hpp"

namespace ultralight_java {
    /**
     * Bridge for com/labymedia/ultralight/plugin/clipboard/UltralightClipboard to ultralight::Clipboard
     */
    class BridgedClipboard : public ultralight::Clipboard, public JNIReferenceWrapper {
    public:
        /**
         * Constructs a new BridgedClipboard wrapping an existing java clipboard.
         *
         * @param env The JNI environment to use for wrapping the file system
         * @param clipboard The clipboard which should be wrapped
         */
        explicit BridgedClipboard(JNIEnv *env, jobject clipboard);

        /**
         * Clears the clipboard
         */
        void Clear() final;

        /**
         * Reads the clipboard content as UTF-16.
         *
         * @return The clipboard content
         */
        ultralight::String16 ReadPlainText() final;

        /**
         * Writes the specified UTF-16 text into the clipboard.
         *
         * @param text The text to write into the clipboard
         */
        void WritePlainText(const ultralight::String16 &text) final;
    };
}