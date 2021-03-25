package com.labymedia.ultralight.lwjgl3.opengl;/*
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

import com.labymedia.ultralight.bitmap.UltralightBitmap;
import com.labymedia.ultralight.bitmap.UltralightBitmapFormat;
import com.labymedia.ultralight.plugin.render.*;
import org.joml.Matrix4f;
import org.lwjgl.opengl.GL11;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.FloatBuffer;
import java.nio.charset.StandardCharsets;
import java.util.*;
import java.util.stream.Collectors;

import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL12.GL_BGRA;
import static org.lwjgl.opengl.GL12.GL_CLAMP_TO_EDGE;
import static org.lwjgl.opengl.GL13.GL_TEXTURE0;
import static org.lwjgl.opengl.GL13.glActiveTexture;
import static org.lwjgl.opengl.GL30.*;
import static org.lwjgl.opengl.GL32.GL_TEXTURE_2D_MULTISAMPLE;
import static org.lwjgl.opengl.GL32.glTexImage2DMultisample;

public class GPUDriverGL implements UltralightGPUDriver {

    public static final int KCOMMANDTYPE_CLEARRENDERBUFFER = 0;
    public static final int KCOMMANDTYPE_DRAWGEOMETRY = 1;

    public static final short KSHADERTYPE_FILL = 0;
    public static final short KSHADERTYPE_FILLPATH = 1;

    private final Map<Short, ProgramEntry> programs = new HashMap<>();
    private final List<UltralightCommand> commands = new LinkedList<>();
    private final Map<Long, TextureEntry> textureMap = new HashMap<>();
    private final Map<Long, GeometryEntry> geometryMap = new HashMap<>();
    private final Map<Long, RenderBufferEntry> renderBufferMap = new HashMap<>();
    private final GPUContextGL context;
    private long nextTextureId = 1;
    private long nextRenderBufferId = 1;
    private long nextGeometryId = 1;
    private int batchCount;
    private long curProgramId;

    public GPUDriverGL(float scale, boolean vsync, boolean msaa) {
        this.context = new GPUContextGL(scale, vsync, msaa);
    }

    public boolean hasCommandsPending() {
        return !this.commands.isEmpty();
    }

    public void drawCommandList() {
        if (this.commands.isEmpty())
            return;

        batchCount = 0;
        for (UltralightCommand command : commands) {
            if (command.commandType == KCOMMANDTYPE_DRAWGEOMETRY)
                drawGeometry(command.geometryId, command.indicesCount, command.indicesOffset, command.gpuState);
            else if (command.commandType == KCOMMANDTYPE_CLEARRENDERBUFFER)
                clearRenderBuffer(command.gpuState.renderBufferId);
            batchCount++;
        }

        commands.clear();
    }

    private void clearRenderBuffer(long renderBufferId) {
        bindRenderBuffer(renderBufferId);
        glDisable(GL_SCISSOR_TEST);
        CHECK_GL();
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        CHECK_GL();
        glClear(GL_COLOR_BUFFER_BIT);
        CHECK_GL();
    }

    private void bindRenderBuffer(long renderBufferId) {
        if (renderBufferId == 0) {
            // Render buffer id '0' is reserved for window's backbuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            return;
        }

        createFBOIfNeededForActiveContext(renderBufferId);

        RenderBufferEntry entry = renderBufferMap.get(renderBufferId);

        FBOEntry fboEntry = entry.fboMap.get(glfwGetCurrentContext());
        if (fboEntry == null) return;

        if (context.msaaEnabled()) {
            // We use the MSAA FBO when doing multisampled rendering.
            // The other FBO (entry.fbo_id) is used for resolving.
            glBindFramebuffer(GL_FRAMEBUFFER, Math.toIntExact(fboEntry.msaaFboId));
            fboEntry.needsResolve = true;
        } else {
            glBindFramebuffer(GL_FRAMEBUFFER, Math.toIntExact(fboEntry.fboId));
        }

        CHECK_GL();
    }

    private void createFBOIfNeededForActiveContext(long renderBufferId) {

    }

    private void drawGeometry(long geometryId, long indicesCount, long indicesOffset, UltralightGPUState state) {
        glfwMakeContextCurrent(context.activeWindow());

        if (programs.isEmpty())
            loadPrograms();

        bindRenderBuffer(state.renderBufferId);

        setViewport(state.viewportWidth, state.viewportHeight);

        GeometryEntry geometry = geometryMap.computeIfAbsent(geometryId, unused -> new GeometryEntry());
        selectProgram(state.shaderType);
        updateUniforms(state);

        CHECK_GL();

        createVAOIfNeededForActiveContext(geometryId);
        long vaoEntry = geometry.vaoMap.get(glfwGetCurrentContext());
        glBindVertexArray(Math.toIntExact(vaoEntry));
        CHECK_GL();

        bindTexture(0, state.texture1Id);
        bindTexture(1, state.texture2Id);
        bindTexture(2, state.texture3Id);

        CHECK_GL();

        if (state.enable_scissor) {
            glEnable(GL_SCISSOR_TEST);
    const IntRect & r = state.scissor_rect;
            glScissor(r.left, r.top, (r.right - r.left), (r.bottom - r.top));
        } else {
            glDisable(GL_SCISSOR_TEST);
        }

        if (state.enable_blend)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);
        CHECK_GL();
        glDrawElements(GL_TRIANGLES, indices_count, GL_UNSIGNED_INT,
                (GLvoid *) (indices_offset * sizeof(unsigned int)));
        CHECK_GL();
        glBindVertexArray(0);

#if ENABLE_OFFSCREEN_GL
        auto & rbuf = render_buffer_map[state.render_buffer_id];
        if (rbuf.bitmap)
            rbuf.needs_update = true;
#endif

        batch_count_++;

        CHECK_GL();
    }

    private void bindTexture(int texture_unit, long texture_id) {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        bindUltralightTexture(texture_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        CHECK_GL();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        CHECK_GL();
    }

    private void bindUltralightTexture(long textureId) {
        TextureEntry entry = textureMap.computeIfAbsent(textureId, unused -> new TextureEntry());
        resolveIfNeeded(entry.renderBufferId);
        glBindTexture(GL_TEXTURE_2D, Math.toIntExact(entry.texId));
        CHECK_GL();

    }

    void resolveIfNeeded(long renderBufferId) {
        if (!context.msaaEnabled())
            return;

        if (renderBufferId == 0)
            return;

        RenderBufferEntry renderBufferEntry = renderBufferMap.computeIfAbsent(renderBufferId, unused -> new RenderBufferEntry());
        if (renderBufferEntry.textureId == 0)
            return;

        FBOEntry fboEntry = renderBufferEntry.fboMap.get(glfwGetCurrentContext());
        if (fboEntry == null)
            return;

        TextureEntry textureEntry = textureMap.computeIfAbsent(renderBufferEntry.textureId, unused -> new TextureEntry());
        if (fboEntry.needsResolve) {
            int drawFboId = glGetInteger(GL_DRAW_FRAMEBUFFER_BINDING);
            int readFboId = glGetInteger(GL_READ_FRAMEBUFFER_BINDING);
            CHECK_GL();
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, Math.toIntExact(fboEntry.fboId));
            glBindFramebuffer(GL_READ_FRAMEBUFFER, Math.toIntExact(fboEntry.msaaFboId));
            CHECK_GL();
            glBlitFramebuffer(0, 0, Math.toIntExact(textureEntry.width), Math.toIntExact(textureEntry.height), 0, 0,
                    Math.toIntExact(textureEntry.width), Math.toIntExact(textureEntry.height), GL_COLOR_BUFFER_BIT, GL_NEAREST);
            CHECK_GL();
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFboId);
            glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
            CHECK_GL();
            fboEntry.needsResolve = false;
        }
    }


    private void createVAOIfNeededForActiveContext(long geometryId) {

    }

    void updateUniforms(UltralightGPUState state) {
        boolean flip_y = state.renderBufferId != 0;
        Matrix4f modelViewProjection = applyProjection(state.transformMatrix, state.viewportWidth, state.viewportHeight, flip_y);

        float params[] = new float[]{
                (float) (glfwGetTime() / 1000.0), state.viewportWidth, state.viewportHeight, 1.0f
        };
        setUniform4f("State", params);
        CHECK_GL();
        ultralight::Matrix4x4 mat = modelViewProjection.GetMatrix4x4();
        SetUniformMatrix4fv("Transform", 1, mat.data);
        CHECK_GL();
        SetUniform4fv("Scalar4", 2, & state.uniform_scalar[0]);
        CHECK_GL();
        SetUniform4fv("Vector", 8, & state.uniform_vector[0].value[0]);
        CHECK_GL();
        SetUniform1ui("ClipSize", state.clip_size);
        CHECK_GL();
        SetUniformMatrix4fv("Clip", 8, state.clip[0].data[0]);
        CHECK_GL();
    }

    private void setUniform4f(String name, float[] params) {
        glUniform4fv(glGetUniformLocation(Math.toIntExact(curProgramId), name), params);
    }

    Matrix4f applyProjection(ByteBuffer transform, float screen_width, float screen_height, boolean flip_y) {
        Matrix4f transform_mat = new Matrix4f(transform.asFloatBuffer());

        Matrix result;
        result.SetOrthographicProjection(screen_width, screen_height, flip_y);
        result.Transform(transform_mat);

        return result;
    }

    void SetUniformMatrix4fv(String name, size_t count, FloatBuffer val) {
        glUniformMatrix4fv(glGetUniformLocation(cur_program_id_, name), (GLsizei) count, false, val);
    }

    private void loadPrograms() {
        loadProgram(KSHADERTYPE_FILL);
        loadProgram(KSHADERTYPE_FILLPATH);
    }

    void selectProgram(short type) {
        ProgramEntry programEntry = this.programs.get(type);
        if (programEntry == null) {
            throw new RuntimeException("Missing shader type: " + type);
        }
        curProgramId = programEntry.programId;
        glUseProgram(programEntry.programId);
    }


    private void loadProgram(short type) {
        CHECK_GL();
        ProgramEntry prog = new ProgramEntry();
        if (type == KSHADERTYPE_FILL) {
            prog.vertShaderId = loadShaderFromSource(GL_VERTEX_SHADER,
                    readShader("shader_v2f_c4f_t2f_t2f_d28f.vs"), "shader_v2f_c4f_t2f_t2f_d28f.vert");
            prog.fragShaderId = loadShaderFromSource(GL_FRAGMENT_SHADER,
                    "shader_fill_frag.fs", "shader_fill.frag");
        } else if (type == KSHADERTYPE_FILLPATH) {
            prog.vertShaderId = loadShaderFromSource(GL_VERTEX_SHADER,
                    "shader_v2f_c4f_t2f.vs", "shader_v2f_c4f_t2f.vert");
            prog.fragShaderId = loadShaderFromSource(GL_FRAGMENT_SHADER,
                    "shader_fill_path_frag.fs", "shader_fill_path.frag");
        }

        prog.programId = glCreateProgram();
        glAttachShader(prog.programId, prog.vertShaderId);
        glAttachShader(prog.programId, prog.fragShaderId);

        glBindAttribLocation(prog.programId, 0, "in_Position");
        glBindAttribLocation(prog.programId, 1, "in_Color");
        glBindAttribLocation(prog.programId, 2, "in_TexCoord");

        if (type == KSHADERTYPE_FILL) {
            glBindAttribLocation(prog.programId, 3, "in_ObjCoord");
            glBindAttribLocation(prog.programId, 4, "in_Data0");
            glBindAttribLocation(prog.programId, 5, "in_Data1");
            glBindAttribLocation(prog.programId, 6, "in_Data2");
            glBindAttribLocation(prog.programId, 7, "in_Data3");
            glBindAttribLocation(prog.programId, 8, "in_Data4");
            glBindAttribLocation(prog.programId, 9, "in_Data5");
            glBindAttribLocation(prog.programId, 10, "in_Data6");
        }

        glLinkProgram(prog.programId);
        glUseProgram(prog.programId);

        if (type == KSHADERTYPE_FILL) {
            glUniform1i(glGetUniformLocation(prog.programId, "Texture1"), 0);
            glUniform1i(glGetUniformLocation(prog.programId, "Texture2"), 1);
            glUniform1i(glGetUniformLocation(prog.programId, "Texture3"), 2);
        }

        if (glGetError() != 0)
            throw new RuntimeException("Unable to link shader.\n\tError:" + "\n\tLog: " + glGetProgramInfoLog(prog.programId));

        programs.put(type, prog);
    }


    void setViewport(long width, long height) {
        glViewport(0, 0, Math.toIntExact(width), Math.toIntExact(height));
    }

    private String readShader(String s) {
        return new BufferedReader(
                new InputStreamReader(this.getClass().getClassLoader().getResourceAsStream(s), StandardCharsets.UTF_8))
                .lines()
                .collect(Collectors.joining("\n"));

    }

    static int loadShaderFromSource(int shaderType, String source, String filename) {
        int shader_id = glCreateShader(shaderType);

        glShaderSource(shader_id, source);
        glCompileShader(shader_id);
        int compileStatus = glGetShaderi(shader_id, GL_COMPILE_STATUS);
        if (compileStatus == GL_FALSE)
            throw new RuntimeException("Unable to compile shader. Filename: " + filename + "\n\tError:"
                    + "\n\tLog: " + getShaderLog(shader_id));
        return shader_id;
    }

    static String getShaderLog(int shaderId) {
        return glGetShaderInfoLog(shaderId);
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

        TextureEntry entry = textureMap.computeIfAbsent(textureId, unused -> new TextureEntry());
        int[] texId = new int[1];
        glGenTextures(texId);

        entry.texId = texId[0];

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Math.toIntExact(entry.texId));
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

        TextureEntry entry = textureMap.computeIfAbsent(textureId, (unused) -> new TextureEntry());
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

        RenderBufferEntry entry = renderBufferMap.computeIfAbsent(renderBufferId, (unused) -> new RenderBufferEntry());
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

        long previous_context = glfwGetCurrentContext();

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
        glBufferData(GL_ARRAY_BUFFER, vertices.data, GL_DYNAMIC_DRAW);
        CHECK_GL();

        int[] vboIndices = new int[1];
        glGenBuffers(vboIndices);
        geometry.vboIndices = vboIndices[0];
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.vboIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.data,
                GL_STATIC_DRAW);
        CHECK_GL();

        geometryMap.put(geometryId, geometry);
    }

    @Override
    public void updateGeometry(long geometryId, UltralightVertexBuffer vertices, UltralightIndexBuffer indices) {
        GeometryEntry geometry = geometryMap.get(geometryId);
        CHECK_GL();
        glBindBuffer(GL_ARRAY_BUFFER, geometry.vboVertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.data, GL_DYNAMIC_DRAW);
        CHECK_GL();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.vboIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.data, GL_STATIC_DRAW);
        CHECK_GL();
        CHECK_GL();
    }

    @Override
    public void destroyGeometry(long geometryId) {
        GeometryEntry geometry = geometryMap.get(geometryId);
        CHECK_GL();
        glDeleteBuffers(geometry.vboIndices);
        glDeleteBuffers(geometry.vboVertices);
        CHECK_GL();

        long previous_context = glfwGetCurrentContext();

        for (Map.Entry<Long, Long> entry : geometry.vaoMap.entrySet()) {
            long context = entry.getKey();
            long vao_entry = entry.getValue();
            glfwMakeContextCurrent(context);
            glDeleteVertexArrays(Math.toIntExact(vao_entry));
            CHECK_GL();
        }

        CHECK_GL();
        geometryMap.remove(geometryId);

        glfwMakeContextCurrent(previous_context);
    }

    @Override
    public void updateCommandList(UltralightCommandList list) {
        if (list.data.length != 0) {
            for (UltralightCommand datum : list.data) {
                this.commands.add(datum);
            }
        }
    }
}
