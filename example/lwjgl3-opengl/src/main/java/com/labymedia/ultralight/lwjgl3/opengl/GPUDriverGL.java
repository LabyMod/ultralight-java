package com.labymedia.ultralight.lwjgl3.opengl;

import com.labymedia.ultralight.bitmap.UltralightBitmap;
import com.labymedia.ultralight.bitmap.UltralightBitmapFormat;
import com.labymedia.ultralight.plugin.render.*;
import org.lwjgl.glfw.GLFW;
import org.lwjgl.opengl.GL11;

import java.nio.ByteBuffer;
import java.util.HashMap;
import java.util.Map;

import static com.sun.org.apache.xalan.internal.xsltc.compiler.Constants.FATAL;
import static org.lwjgl.glfw.GLFW.glfwMakeContextCurrent;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL12.GL_BGRA;
import static org.lwjgl.opengl.GL12.GL_CLAMP_TO_EDGE;
import static org.lwjgl.opengl.GL13.GL_TEXTURE0;
import static org.lwjgl.opengl.GL13.glActiveTexture;
import static org.lwjgl.opengl.GL30.*;
import static org.lwjgl.opengl.GL32.GL_TEXTURE_2D_MULTISAMPLE;
import static org.lwjgl.opengl.GL32.glTexImage2DMultisample;

public class GPUDriverGL implements UltralightGPUDriver {

    private final Map<Long, TextureEntry> textureMap = new HashMap<>();
    private final Map<Long, RenderBufferEntry> renderBufferMap = new HashMap<>();
    private final GPUContextGL context;
    private long nextTextureId = 1;
    private long nextRenderBufferId = 1;
    private long nextGeometryId = 1;


    public GPUDriverGL(float scale, boolean vsync, boolean msaa) {
        this.util = GPUDriverUtil.create();
        this.contextHandle = util.createOpenGLContext(scale, vsync, msaa);
        this.driverHandle = util.getDriverFromContext(this.contextHandle);
    }

    @Override
    public void beginSynchronize() {
    }

    @Override
    public void endSynchronize() {
    }

    @Override
    public long nextTextureId() {
        return this.nextTextureId++;
    }

    @Override
    public long nextRenderBufferId() {
        return this.nextRenderBufferId++;
    }

    @Override
    public long nextGeometryId() {
        return this.nextGeometryId++;
    }

    @Override
    public void createTexture(long textureId, UltralightBitmap bitmap) {
        if (bitmap.isEmpty()) {
            this.createFBOTexture(textureId, bitmap);
            return;
        }

        CHECK_GL();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        CHECK_GL();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        CHECK_GL();

        TextureEntry entry = textureMap.get(textureId);
        int[] texId = new int[1];
        glGenTextures(texId);

        entry.texId = texId[0];

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, entry.texId);
        CHECK_GL();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, Math.toIntExact(bitmap.rowBytes() / bitmap.bpp()));
        CHECK_GL();
        if (bitmap.format() == UltralightBitmapFormat.A8_UNORM) {

            ByteBuffer pixels = bitmap.lockPixels();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, (int) bitmap.width(), (int) bitmap.height(), 0,
                    GL_RED, GL_UNSIGNED_BYTE, pixels);
            bitmap.unlockPixels();


        } else if (bitmap.format() == UltralightBitmapFormat.BGRA8_UNORM_SRGB) {

            ByteBuffer pixels = bitmap.lockPixels();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int) bitmap.width(), (int) bitmap.height(), 0,
                    GL_BGRA, GL_UNSIGNED_BYTE, pixels);
            bitmap.unlockPixels();

        } else {
            throw new RuntimeException("Unhandled texture format: " + bitmap.format());
        }

        CHECK_GL();
        glGenerateMipmap(GL_TEXTURE_2D);
        CHECK_GL();
    }

    void CHECK_GL() {
        if (GL11.glGetError() != GL11.GL_NO_ERROR) {
            throw new RuntimeException();
        }
    }

    private void createFBOTexture(long textureId, UltralightBitmap bitmap) {
        CHECK_GL();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        CHECK_GL();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        CHECK_GL();

        TextureEntry entry = textureMap.get(textureId);
        entry.width = bitmap.width();
        entry.height = bitmap.height();

        // Allocate a single-sampled texture
        int[] texId = new int[1];
        glGenTextures(texId);
        entry.texId = texId[0];
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, (int) entry.texId);

        // Allocate texture in linear space.
        // We will convert back to sRGB for monitor when binding renderbuffer 0
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int) entry.width, (int) entry.height, 0,
                GL_BGRA, GL_UNSIGNED_BYTE, (ByteBuffer) null);

        if (context.msaaEnabled()) {
            // Allocate the multisampled texture
            int[] msaaTexId = new int[1];
            glGenTextures(msaaTexId);
            entry.msaaTexId = msaaTexId[0];
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, (int) entry.msaaTexId);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, (int) entry.width, (int) entry.height, false);
        }

        CHECK_GL();
        glGenerateMipmap(GL_TEXTURE_2D);
        CHECK_GL();
    }

    @Override
    public void updateTexture(long textureId, UltralightBitmap bitmap) {
        glActiveTexture(GL_TEXTURE0 + 0);
        TextureEntry entry = this.textureMap.get(textureId);
        glBindTexture(GL_TEXTURE_2D, (int) entry.texId);
        CHECK_GL();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_UNPACK_ROW_LENGTH, Math.toIntExact(bitmap.rowBytes() / bitmap.bpp()));

        if (!bitmap.isEmpty()) {
            if (bitmap.format() == UltralightBitmapFormat.A8_UNORM) {
                ByteBuffer pixels = bitmap.lockPixels();
                glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, (int) bitmap.width(), (int) bitmap.height(), 0,
                        GL_RED, GL_UNSIGNED_BYTE, pixels);
                bitmap.unlockPixels();

            } else if (bitmap.format() == UltralightBitmapFormat.BGRA8_UNORM_SRGB) {
                ByteBuffer pixels = bitmap.lockPixels();
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int) bitmap.width(), (int) bitmap.height(), 0,
                        GL_BGRA, GL_UNSIGNED_BYTE, pixels);
                bitmap.unlockPixels();
            } else {
                throw new RuntimeException("Unhandled texture format: " + bitmap.format());
            }

            CHECK_GL();
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        CHECK_GL();
    }

    @Override
    public void destroyTexture(long textureId) {
        TextureEntry entry = textureMap.get(textureId);

        glDeleteTextures(Math.toIntExact(entry.texId));
        CHECK_GL();
        if (entry.msaaTexId != 0L)
            glDeleteTextures(Math.toIntExact(entry.msaaTexId));
        CHECK_GL();
    }

    @Override
    public void createRenderBuffer(long renderBufferId, UltralightRenderBuffer buffer) {
        if (renderBufferId == 0) {
            System.out.println("Should not be reached! Render Buffer ID 0 is reserved for default framebuffer.");
            return;
        }

        RenderBufferEntry entry = renderBufferMap.get(renderBufferId);
        entry.textureId = buffer.textureId;

        TextureEntry textureEntry = textureMap.get(buffer.textureId);
        textureEntry.renderBufferId = renderBufferId;

        // We don't actually create FBOs here-- they are lazily-created
        // for each active window during BindRenderBuffer (this is because
        // FBOs are not shared between contexts in GL 3.2)
    }

    @Override
    public void destroyRenderBuffer(long renderBufferId) {
        if (renderBufferId == 0)
            return;

        long previous_context = GLFW.glfwGetCurrentContext();

        RenderBufferEntry renderBufferEntry = renderBufferMap.get(renderBufferId);
        for (Map.Entry<Long, FBOEntry> entry : renderBufferEntry.fboMap.entrySet()) {

            long context = entry.getKey();
            FBOEntry fboEntry = entry.getValue();
            glfwMakeContextCurrent(context);
            glDeleteFramebuffers(Math.toIntExact(fboEntry.fboId));
            CHECK_GL();
            if (this.context.msaaEnabled())
                glDeleteFramebuffers(Math.toIntExact(fboEntry.msaaFboId));
            CHECK_GL();
        }

        if (context.isEnableOffscreenGl()) {

            // Clean up PBOs if a bitmap is bound
            if (renderBufferEntry.bitmap != null)
                glDeleteBuffers(Math.toIntExact(renderBufferEntry.pboId));
        }
        CHECK_GL();
        renderBufferMap.remove(renderBufferId);

        glfwMakeContextCurrent(previous_context);
    }

    @Override
    public void createGeometry(long geometryId, UltralightVertexBuffer vertices, UltralightIndexBuffer indices) {
        GeometryEntry geometry = new GeometryEntry();
        geometry.vertexFormat = vertices.format;

        int[] vboVertices = new int[1];
        glGenBuffers(vboVertices);
        geometry.vboVertices = vboVertices[0];

        glBindBuffer(GL_ARRAY_BUFFER, geometry.vboVertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.size, vertices.data, GL_DYNAMIC_DRAW);
        CHECK_GL();

        glGenBuffers(1, & geometry.vbo_indices);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.vbo_indices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size, indices.data,
                GL_STATIC_DRAW);
        CHECK_GL();

        geometry_map[geometry_id] = geometry;
    }

    @Override
    public void updateGeometry(long geometryId, UltralightVertexBuffer vertices, UltralightIndexBuffer indices) {

    }

    @Override
    public void destroyGeometry(long geometryId) {

    }

    @Override
    public void updateCommandList(UltralightCommandList list) {

    }
}
