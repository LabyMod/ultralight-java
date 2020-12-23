package com.labymedia.ultralight.lwjgl3.opengl.js;

import java.util.Arrays;
import java.util.List;

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
}
