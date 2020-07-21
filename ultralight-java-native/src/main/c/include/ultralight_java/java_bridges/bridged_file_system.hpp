#pragma once

#include <Ultralight/Ultralight.h>

#include "ultralight_java/util/jni_reference_wrapper.hpp"

namespace ultralight_java {
    /**
     * Bridge for net/janrupf/ultralight/plugin/filesystem/UltralightFileSystem to ultralight::FileSystem
     */
    class BridgedFileSystem : public ultralight::FileSystem, public JNIReferenceWrapper {
    public:
        /**
         * Constructs a new BridgedFileSystem wrapping an existing java file system.
         *
         * @param env The JNI environment to use for wrapping the file system
         * @param file_system The file system which should be wrapped
         */
        explicit BridgedFileSystem(JNIEnv *env, jobject file_system);

        /**
         * Determines if a file exists with the given path.
         *
         * @param path The path to check for a file at
         * @return Whether the file exists
         */
        bool FileExists(const ultralight::String16 &path) final;

        /**
         * Retrieves the size of an opened file.
         *
         * @param handle The handle of the opened file
         * @param result Output to write the size into
         * @return Whether the operation succeeded, result is only valid if true is returned
         */
        bool GetFileSize(ultralight::FileHandle handle, int64_t &result) final;

        /**
         * Retrieves the mime type of a file with the given path.
         *
         * @param path The path of the file to retrieve the mime type for
         * @param result Output to write the mime type into
         * @return Whether the operation succeeded, result is only valid if true is returned
         */
        bool GetFileMimeType(const ultralight::String16 &path, ultralight::String16 &result) final;

        /**
         * Opens the file at the given path.
         *
         * @param path The path of the file to open
         * @param open_for_writing Whether the file should be opened for writing
         * @return A handle for the opened file, or ultralight::invalidFileHandle in case of a failure
         */
        ultralight::FileHandle OpenFile(const ultralight::String16 &path, bool open_for_writing) final;

        /**
         * Closes the given file.
         *
         * @param handle The handle of the file to close
         */
        void CloseFile(ultralight::FileHandle &handle) final;

        /**
         * Reads data from the given file.
         *
         * @param handle The handle of the file to read data from
         * @param data Output buffer where data should be written in to
         * @param length The amount of bytes to read, data has at least this size
         * @return The amount of bytes read, or -1 on failure
         */
        int64_t ReadFromFile(ultralight::FileHandle handle, char *data, int64_t length) final;
    };
} // namespace ultralight_java