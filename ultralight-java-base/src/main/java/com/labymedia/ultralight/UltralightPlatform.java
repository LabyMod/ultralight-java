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

package com.labymedia.ultralight;

import com.labymedia.ultralight.annotation.NativeCall;
import com.labymedia.ultralight.annotation.NativeType;
import com.labymedia.ultralight.annotation.Unsigned;
import com.labymedia.ultralight.config.UltralightConfig;
import com.labymedia.ultralight.ffi.ObjectWithHandle;
import com.labymedia.ultralight.plugin.clipboard.UltralightClipboard;
import com.labymedia.ultralight.plugin.filesystem.UltralightFileSystem;
import com.labymedia.ultralight.plugin.logging.UltralightLogger;

/**
 * Platform singleton to configure Ultralight and provide user-defined implementations for various platform operations.
 * <p>
 * All of these settings and user-defined interfaces should be set BEFORE creating the Renderer.
 */
@NativeType("ultralight::Platform")
public class UltralightPlatform implements ObjectWithHandle {
    /**
     * This fields holds a pointer to the underlying instance of the C++ object.
     */
    @NativeType("ultralight::Platform *")
    @Unsigned
    private final long handle;

    @NativeCall
    private UltralightPlatform(long handle) {
        this.handle = handle;
    }

    /**
     * Get the {@link UltralightPlatform} singleton.
     *
     * @return The {@link UltralightPlatform} singleton
     */
    public static native UltralightPlatform instance();

    /**
     * Set the Config.
     *
     * @param config The new config
     */
    public native void setConfig(UltralightConfig config);

    /**
     * Sets the internal font loader implementation to the platform dependent one.
     */
    public native void usePlatformFontLoader();

    /**
     * Sets the internal file system implementation to the platform dependent one.
     *
     * @param basePath The path to resolve `file:///` URL's relative to
     */
    public native void usePlatformFileSystem(String basePath);

    /**
     * Set the File System (will be used for all file system operations).
     *
     * @param fileSystem The file system to use
     */
    public native void setFileSystem(UltralightFileSystem fileSystem);

    /**
     * Set the Clipboard (will be used for all clipboard operations).
     *
     * @param clipboard A user-defined Clipboard implementation
     */
    public native void setClipboard(UltralightClipboard clipboard);

    // TODO: Font loader support

    // TODO: GPU driver support

    // TODO: Surface factory support

    /**
     * Sets the internal logger implementation to the provided one.
     *
     * @param logger The logger to use
     */
    public native void setLogger(UltralightLogger logger);

    /**
     * Retrieves the native underlying pointer to the Platform object.
     *
     * @return The native underlying pointer
     */
    @Override
    public @NativeType("ultralight::Platform *") @Unsigned long getHandle() {
        return handle;
    }
}
