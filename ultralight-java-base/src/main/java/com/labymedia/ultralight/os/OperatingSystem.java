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

package com.labymedia.ultralight.os;

import java.util.function.Function;

/**
 * Enum representing know operating systems.
 */
public enum OperatingSystem {
    /**
     * Some kind of Linux or Unix platform.
     */
    LINUX(s -> "lib" + s + ".so"),

    /**
     * Some kind of Windows platform.
     */
    WINDOWS(s -> s + ".dll"),

    /**
     * Some kind of MacOS platform
     */
    MAC_OS(s -> "lib" + s + ".dylib"),

    /**
     * OS which name could not be identified. Mapping library names on it will
     * map them like on linux, since this has the best chance of succeeding.
     */
    UNKNOWN(LINUX::mapLibraryName);

    private final Function<String, String> libraryNameMapper;

    /**
     * Constructs a new {@link OperatingSystem} with the given library name mapper.
     *
     * @param libraryNameMapper Function used to map library names to the platform name
     */
    OperatingSystem(Function<String, String> libraryNameMapper) {
        this.libraryNameMapper = libraryNameMapper;
    }

    /**
     * Maps the given library name to the platform library name.
     *
     * @param libraryName The library name to map
     * @return The mapped name
     */
    public String mapLibraryName(String libraryName) {
        return libraryNameMapper.apply(libraryName);
    }

    /**
     * Guesses the platform this application is running on.
     *
     * @return A best guess of the platform this application is running on
     */
    public static OperatingSystem get() {
        String systemName = System.getProperty("os.name", "unknown").toLowerCase();

        if (systemName.contains("nux") || systemName.contains("nix")) {
            return LINUX;
        } else if (systemName.contains("mac")) {
            return MAC_OS;
        } else if (systemName.contains("windows")) {
            return WINDOWS;
        } else {
            return UNKNOWN;
        }
    }
}
