package com.labymedia.ultralight.lwjgl3.opengl;

import com.labymedia.ultralight.bitmap.UltralightBitmap;

import java.util.HashMap;
import java.util.Map;

public class RenderBufferEntry {
    Map<Long, FBOEntry> fboMap = new HashMap<>();
    long textureId;
    UltralightBitmap bitmap;
    long pboId;
    boolean isBitmapDirty;
    boolean isFirstDraw;
    boolean needsUpdate;
}
