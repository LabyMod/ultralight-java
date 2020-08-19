package net.janrupf.ultralight.plugin.render;

import net.janrupf.ultralight.annotation.NativeType;

import java.util.ArrayList;

@NativeType("ultralight::VertexBuffer")
public class UltralightVertexBuffer extends ArrayList<Short> {

    private @NativeType("ultralight::VertexBufferFormat") UltralightVertexBufferFormat format;

    public void setFormat(UltralightVertexBufferFormat format) {
        this.format = format;
    }

    public UltralightVertexBufferFormat getFormat() {
        return this.format;
    }

}
