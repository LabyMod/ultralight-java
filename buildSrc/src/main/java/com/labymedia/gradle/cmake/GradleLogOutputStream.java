package com.labymedia.gradle.cmake;


import org.gradle.api.logging.LogLevel;
import org.gradle.api.logging.Logger;

import java.io.OutputStream;

public class GradleLogOutputStream extends OutputStream {
    private String prefix;
    private Logger logger;
    private LogLevel level;
    private StringBuilder buffer;

    public GradleLogOutputStream(String prefix, Logger logger, LogLevel level) {
        this.prefix = prefix;
        this.logger = logger;
        this.level = level;
        this.buffer = new StringBuilder(1024);
        this.buffer.append("[").append(prefix).append("] ");
    }

    public void write(int b) {
        char c = (char) b;
        if (c == '\n') {
            this.logger.log(this.level, this.buffer.toString());
            this.buffer.setLength(0);
            this.buffer.append("[").append(this.prefix).append("] ");
        } else {
            this.buffer.append(c);
        }

    }
}
