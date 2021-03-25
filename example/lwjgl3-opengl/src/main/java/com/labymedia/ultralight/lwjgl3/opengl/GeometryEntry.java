package com.labymedia.ultralight.lwjgl3.opengl;

import com.labymedia.ultralight.plugin.render.UltralightVertexBufferFormat;

import java.util.HashMap;
import java.util.Map;

public class GeometryEntry {
    Map<Long, Long> vaoMap = new HashMap<>();
    UltralightVertexBufferFormat vertexFormat;
    int vboVertices = 0; // VBO id for vertices
    int vboIndices = 0; // VBO id for indices
}
