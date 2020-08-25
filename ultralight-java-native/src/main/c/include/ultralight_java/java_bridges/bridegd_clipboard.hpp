#pragma once

#include <Ultralight/platform/Clipboard.h>

#include "ultralight_java/util/jni_reference_wrapper.hpp"

namespace ultralight_java {
    /**
     * Bridge for net/labymedia/ultralight/plugin/clipboard/UltralightClipboard to ultralight::Clipboard
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