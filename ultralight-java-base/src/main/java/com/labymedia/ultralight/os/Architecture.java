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

/**
 * Enum representing known computer architectures.
 */
public enum Architecture {
    /**
     * amd64/x86_64 architecture, most common one for home PC's
     */
    AMD64(64),

    /**
     * i386/x86 architecture, most common one for older home PC's
     */
    I386(32),

    /**
     * Unknown architecture
     */
    UNKNOWN(-1);

    private final int bits;

    /**
     * Constructs a new {@link Architecture} with the given amount of bits per pointer.
     *
     * @param bits The amount of bits of a pointer on this architecture
     */
    Architecture(int bits) {
        this.bits = bits;
    }

    /**
     * Retrieves the amount of bits a pointer has on this platform.
     *
     * @return The amount of bits of a pointer, or {@code -1} if unknown
     */
    public int getBits() {
        return bits;
    }

    /**
     * Guesses the architecture this application is running on.
     *
     * @return A best guess of the architecture
     */
    public static Architecture get() {
        String arch = System.getProperty("os.arch", "unknown").toLowerCase();
        String dataModel = System.getProperty("sun.arch.data.model", "unknown").toLowerCase();

        if (arch.equals("unknown") && dataModel.equals("unknown")) {
            return UNKNOWN;
        } else if (arch.contains("64") || dataModel.contains("64")) {
            // TODO: Technically incorrect, for now we just assume its some kind of amd64 PC
            return AMD64;
        } else {
            // TODO: Technically incorrect, for now just assume its some kind of i386 PC
            return I386;
        }
    }
}
