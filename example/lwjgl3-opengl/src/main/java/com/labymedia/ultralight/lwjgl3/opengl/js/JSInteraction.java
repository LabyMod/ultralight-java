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

package com.labymedia.ultralight.lwjgl3.opengl.js;

import java.util.Arrays;
import java.util.List;
import java.util.function.Consumer;

/**
 * Example class containing methods which Javascript can interact with.
 */
public class JSInteraction {
    private final String[] messages;

    /**
     * Javascript can call varargs method as if they would be native.
     *
     * @param messages The message to store
     */
    public JSInteraction(String... messages) {
        this.messages = messages;
    }

    /**
     * Javascript can work on interface types without any issues.
     *
     * @return The stored messages as a list
     */
    public List<String> getMessageList() {
        return Arrays.asList(messages);
    }

    /**
     * Java arrays are translated to Javascript arrays automatically.
     *
     * @return The stored messages
     */
    public String[] getMessageArray() {
        return messages;
    }

    /**
     * Javascript methods can be automatically convert to Java functional interfaces as long as they are annotated with
     * {@link FunctionalInterface}.
     *
     * @param consumer The consumer to pass the messages to
     */
    public void useConsumer(Consumer<String[]> consumer) {
        consumer.accept(messages);
    }
}
