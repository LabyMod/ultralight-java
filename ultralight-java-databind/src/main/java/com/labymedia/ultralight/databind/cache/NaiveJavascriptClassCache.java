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

package com.labymedia.ultralight.databind.cache;

import com.labymedia.ultralight.javascript.JavascriptClass;

import java.util.HashMap;
import java.util.Map;

/**
 * Simple, {@link HashMap} based implementation of a {@link JavascriptClassCache}.
 */
public final class NaiveJavascriptClassCache implements JavascriptClassCache {
    private final Map<String, JavascriptClass> cache;

    /**
     * Constructs a new empty {@link NaiveJavascriptClassCache}.
     */
    public NaiveJavascriptClassCache() {
        this.cache = new HashMap<>();
    }

    @Override
    public JavascriptClass get(String name) {
        return cache.get(name);
    }

    @Override
    public JavascriptClass put(String name, JavascriptClass javascriptClass) {
        cache.put(name, javascriptClass);
        return javascriptClass;
    }

    @Override
    public JavascriptClass delete(String name) {
        return cache.remove(name);
    }

    @Override
    public boolean contains(String name) {
        return cache.containsKey(name);
    }
}
