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

package com.labymedia.ultralight.example.base;

import com.labymedia.ultralight.UltralightJava;
import com.labymedia.ultralight.UltralightLoadException;
import com.labymedia.ultralight.gpu.UltralightGPUDriverNativeUtil;
import org.lwjgl.glfw.GLFW;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;

public abstract class UltralightExampleBase {

    private final UltralightExampleConfiguration ultralightExampleConfiguration;

    public UltralightExampleBase() {
        try {

            this.ultralightExampleConfiguration = new UltralightExampleConfiguration();
            this.setupNatives();

            this.configure(this.ultralightExampleConfiguration);

            // Example resources
            extractResources();
            this.begin();
        } catch (UltralightLoadException exception) {
            //Process will end here. Just wrapped in a RuntimeException so no example has to create a constructor
            throw new RuntimeException(exception);
        }
    }

    private void setupNatives() throws UltralightLoadException {
        // Get a directory to put natives into
        Path nativesDir = Paths.get(".");

        // Get the existing native library path
        String libraryPath = System.getProperty("java.library.path");
        if (libraryPath != null) {
            // There is a path set already, append our natives dir
            libraryPath += File.pathSeparator + nativesDir.toAbsolutePath().toString();
        } else {
            // There is no path set, make our natives dir the current path
            libraryPath = nativesDir.toAbsolutePath().toString();
        }

        // Set the path back
        System.setProperty("java.library.path", libraryPath);

        // Extract the natives
        //
        // This only extracts the native library for ultralight-java-base, but not the other Ultralight libraries.
        // It is your task to get them into the run directory, possibly by extracting them on your own.
        UltralightJava.extractNativeLibrary(nativesDir);

        // Load the native libraries from the given directory. This method makes sure everything is loaded in the
        // correct order. If you want to manually load all natives, either don't use this function or pass 'false' as
        // the second parameter.
        UltralightJava.load(nativesDir);
        UltralightGPUDriverNativeUtil.extractNativeLibrary(nativesDir);

    }

    public void configure(UltralightExampleConfiguration ultralightExampleConfiguration) {
    }

    public void begin() {
    }

    /**
     * Helper function to set up the run directory with jar resources.
     */
    private void extractResources() {
        try {
            for (String path : this.ultralightExampleConfiguration.getResourcesToExtract()) {
                Files.copy(
                        UltralightExampleBase.class.getResourceAsStream("/" + path),
                        Paths.get("./" + path),
                        StandardCopyOption.REPLACE_EXISTING
                );
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

}
