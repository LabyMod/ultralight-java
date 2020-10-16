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

package com.labymedia.ultralight.lwjgl3.opengl;

import com.labymedia.ultralight.plugin.filesystem.UltralightFileSystem;

import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.*;
import java.util.HashMap;
import java.util.Map;

/**
 * Test file system implementation mapping the OS file system 1-to-1.
 * <p>
 * This implementation is based on NIO {@link FileChannel}s, as they can be used for easy mapping.
 * <p>
 * For debugging purposes this implementation contains verbose logging.
 */
public class TestFileSystem implements UltralightFileSystem {
    private long nextFileHandle;
    private final Map<Long, FileChannel> openFiles = new HashMap<>();

    @Override
    public boolean fileExists(String path) {
        log(false, "Checking if %s exists", path);
        Path realPath = getPath(path);
        boolean exists = realPath != null && Files.exists(realPath);
        log(false, "%s %s", path, exists ? "exists" : "does not exist");
        return exists;
    }

    @Override
    public long getFileSize(long handle) {
        log(false, "Retrieving file size of handle %d", handle);
        FileChannel channel = openFiles.get(handle);
        if(channel == null) {
            log(true, "Failed to retrieve file size of handle %d, it was invalid", handle);
            return -1;
        } else {
            try {
                long size = channel.size();
                log(false, "File size of handle %d is %d", handle, size);
                return size;
            } catch(IOException e) {
                log(true, "Exception while retrieving size of handle %d", handle);
                e.printStackTrace();
                return -1;
            }
        }
    }

    @Override
    public String getFileMimeType(String path) {
        log(false, "Retrieving mime type of %s", path);
        Path realPath = getPath(path);
        if(realPath == null) {
            log(true, "Failed to retrieve mime type of %s, path was invalid", path);
            return null;
        }

        try {
            String mimeType = Files.probeContentType(realPath);
            log(false, "Mime type of %s is %s", path, mimeType);
            return mimeType;
        } catch(IOException e) {
            log(true, "Exception while retrieving mime type of %s", path);
            e.printStackTrace();
            return null;
        }
    }

    @Override
    public long openFile(String path, boolean openForWriting) {
        log(false, "Opening file %s for %s", path, openForWriting ? "writing" : "reading");
        Path realPath = getPath(path);
        if(realPath == null) {
            log(true, "Failed to open %s, the path is invalid", path);
            return INVALID_FILE_HANDLE;
        }

        FileChannel channel;
        try {
            channel = FileChannel.open(realPath, openForWriting ? StandardOpenOption.WRITE : StandardOpenOption.READ);
        } catch(IOException e) {
            log(true, "Exception while opening %s", path);
            e.printStackTrace();
            return INVALID_FILE_HANDLE;
        }

        if(nextFileHandle == INVALID_FILE_HANDLE) {
            nextFileHandle = INVALID_FILE_HANDLE + 1;
        }

        long handle = nextFileHandle++;
        openFiles.put(handle, channel);
        log(false, "Opened %s as handle %d", path, handle);
        return handle;
    }

    @Override
    public void closeFile(long handle) {
        log(false, "Closing handle %d", handle);
        FileChannel channel = openFiles.get(handle);
        if(channel != null) {
            try {
                channel.close();
                log(false, "Handle %d has been closed", handle);
            } catch(IOException e) {
                log(true, "Exception while closing handle %d", handle);
                e.printStackTrace();
            }
        } else {
            log(false, "Failed to close handle %d, it was invalid", handle);
        }
    }

    @Override
    public long readFromFile(long handle, ByteBuffer data, long length) {
        log(false, "Trying to read %d bytes from handle %d", length, handle);
        FileChannel channel = openFiles.get(handle);
        if(channel == null) {
            log(true, "Failed to read %d bytes from handle %d, it was invalid", length, handle);
            return -1;
        }

        if(length > Integer.MAX_VALUE) {
            log(true, "Failed to read %d bytes from handle %d, the size exceeded the limit", length, handle);
            // Not supported yet, marked as TODO
            new UnsupportedOperationException().printStackTrace();
            return -1;
        }

        try {
            long read = channel.read((ByteBuffer) data.slice().limit((int) length));
            log(false, "Read %d bytes out of %d requested from handle %d", read, length, handle);
            return read;
        } catch(IOException e) {
            log(true, "Exception occurred while reading %d bytes from handle %d", length, handle);
            e.printStackTrace();
            return -1;
        }
    }

    /**
     * Helper method to scratch malformed paths
     *
     * @param path The path to convert to an NIO path
     * @return The converted path, or {@code null}, if the path failed to convert
     */
    private Path getPath(String path) {
        try {
            return Paths.get(path);
        } catch(InvalidPathException e) {
            return null;
        }
    }

    /**
     * Logs a message to the console.
     *
     * @param error Whether this is an error message
     * @param fmt The format string
     * @param args Arguments to format the string with
     */
    private void log(boolean error, String fmt, Object ...args) {
        String message = String.format(fmt, args);
        if(error) {
            System.err.println("[ERROR/FileSystem] " + message);
        } else {
            System.out.println("[INFO/FileSystem] " + message);
        }
    }
}
