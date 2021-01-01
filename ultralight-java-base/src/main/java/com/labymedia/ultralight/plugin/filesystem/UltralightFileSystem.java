/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2020 - 2021 LabyMedia and contributors
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

package com.labymedia.ultralight.plugin.filesystem;

import com.labymedia.ultralight.UltralightPlatform;

import java.nio.ByteBuffer;

/**
 * FileSystem interface.
 * <p>
 * This is used for loading File URLs (eg, file:///page.html). If you don't implement this interface, you will not be
 * able to load any File URLs.
 * <p>
 * This is intended to be implemented by users and defined before creating the Renderer.
 *
 * @see UltralightPlatform#setFileSystem(UltralightFileSystem)
 */
public interface UltralightFileSystem {
    /**
     * Used as the value for invalid file handles
     */
    long INVALID_FILE_HANDLE = -1;

    /**
     * Check if file path exists.
     *
     * @param path The path to check for a file at
     * @return {@code true} if the file exists, {@code false} otherwise
     */
    boolean fileExists(String path);

    /**
     * Get file size of previously opened file.
     *
     * @param handle The handle of the file to get the size of
     * @return The file size in bytes, or -1 (and lower) to indicate a failure
     */
    long getFileSize(long handle);

    /**
     * Get file mime type (eg "text/html").
     *
     * @param path The path to check the mime type for
     * @return The mime type of the file at path, or {@code null}, if unknown or in case of a failure
     */
    String getFileMimeType(String path);

    /**
     * Open file path for reading or writing.
     *
     * @param path           The path of the file to open
     * @param openForWriting Whether the file should be opened for writing
     * @return An arbitrary number used by the implementation for file identification, or {@link #INVALID_FILE_HANDLE}
     * on failure
     */
    long openFile(String path, boolean openForWriting);

    /**
     * Close previously-opened file.
     *
     * @param handle The handle of the file to close
     */
    void closeFile(long handle);

    /**
     * Read from currently-opened file.
     * <p>
     * TODO: This is not good, since a {@link ByteBuffer} can only store up to {@link Integer#MAX_VALUE} bytes, but the
     * requested size could be bigger. NIO solves this using an array of {@link ByteBuffer}s, should be considered
     * here too.
     *
     * @param handle The handle of the file to read
     * @param data   Buffer to write read data into
     * @param length The amount of bytes to read from the file
     * @return The amount of bytes read, or -1 on failure
     */
    long readFromFile(long handle, ByteBuffer data, long length);
}
