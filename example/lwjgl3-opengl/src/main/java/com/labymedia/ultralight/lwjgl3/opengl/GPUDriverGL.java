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

import com.labymedia.ultralight.UltralightMatrix;
import com.labymedia.ultralight.UltralightMatrix4x4;
import com.labymedia.ultralight.bitmap.UltralightBitmap;
import com.labymedia.ultralight.bitmap.UltralightBitmapFormat;
import com.labymedia.ultralight.math.IntRect;
import com.labymedia.ultralight.plugin.render.*;
import org.lwjgl.opengl.GL11;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;
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
import static org.lwjgl.opengl.GL43.GL_DEBUG_OUTPUT_SYNCHRONOUS;
import static org.lwjgl.opengl.GLUtil.setupDebugMessageCallback;

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
    private long curProgramId;

    public GPUDriverGL() {
        this.context = new GPUContextGL();
    }

    public GPUContextGL getContext() {
        return context;
    }

    public boolean hasCommandsPending() {
        return !this.commands.isEmpty();
    }

    public void drawCommandList() {
        CHECK_GL();
        if (this.commands.isEmpty())
            return;

        for (UltralightCommand command : commands) {
            if (command.commandType == KCOMMANDTYPE_DRAWGEOMETRY) {
                drawGeometry(command.geometryId, command.indicesCount, command.indicesOffset, command.gpuState);
            } else if (command.commandType == KCOMMANDTYPE_CLEARRENDERBUFFER) {
                clearRenderBuffer(command.gpuState.renderBufferId);
            }
        }

        commands.clear();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        CHECK_GL();
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
        CHECK_GL();
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
        CHECK_GL();
        if (renderBufferId == 0)
            return;

        RenderBufferEntry entry = renderBufferMap.get(renderBufferId);
        if (entry == null) {
            throw new RuntimeException("Error, render buffer entry should exist here.");
        }

        FBOEntry fboEntry = entry.fboMap.get(glfwGetCurrentContext());
        if (fboEntry != null) {
            return;
        }

        entry.fboMap.put(glfwGetCurrentContext(), fboEntry = new FBOEntry());

        fboEntry.fboId = glGenFramebuffers();
        CHECK_GL();
        glBindFramebuffer(GL_FRAMEBUFFER, Math.toIntExact(fboEntry.fboId));
        CHECK_GL();

        TextureEntry textureEntry = textureMap.get(entry.textureId);

        if (context.isEnableOffscreenGl()) {
            if (entry.bitmap != null)
                makeTextureSRGBIfNeeded(entry.textureId);
        }

        glBindTexture(GL_TEXTURE_2D, Math.toIntExact(textureEntry.texId));
        CHECK_GL();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Math.toIntExact(textureEntry.texId), 0);
        CHECK_GL();

        int drawBuffers[] = {
                GL_COLOR_ATTACHMENT0
        };
        glDrawBuffers(drawBuffers);
        CHECK_GL();

        int result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (result != GL_FRAMEBUFFER_COMPLETE)
            throw new RuntimeException("Error creating FBO, this usually fails if your DPI scale is invalid or View dimensions are massive: " + result);
        CHECK_GL();

        if (!context.msaaEnabled()) {
            return;
        }

        // Create MSAA FBO
        fboEntry.msaaFboId = glGenFramebuffers();
        CHECK_GL();
        glBindFramebuffer(GL_FRAMEBUFFER, Math.toIntExact(fboEntry.msaaFboId));
        CHECK_GL();

        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, Math.toIntExact(textureEntry.msaaTexId));
        CHECK_GL();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, Math.toIntExact(textureEntry.msaaTexId), 0);
        CHECK_GL();

        glDrawBuffers(drawBuffers);
        CHECK_GL();

        result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (result != GL_FRAMEBUFFER_COMPLETE)
            throw new RuntimeException("Error creating MSAA FBO, this usually fails if your DPI scale is invalid or View dimensions are massive: " + result);
        CHECK_GL();
    }

    private void makeTextureSRGBIfNeeded(long textureId) {
        CHECK_GL();
        TextureEntry textureEntry = textureMap.computeIfAbsent(textureId, unused -> new TextureEntry());
        if (!textureEntry.isSRGB) {
            // We need to make the primary texture sRGB
            // First, Destroy existing texture.
            glDeleteTextures(Math.toIntExact(textureEntry.texId));
            CHECK_GL();
            // Create new sRGB texture
            textureEntry.texId = glGenTextures();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, Math.toIntExact(textureEntry.texId));
            CHECK_GL();
            glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, (int) textureEntry.width, (int) textureEntry.height, 0,
                    GL_BGRA, GL_UNSIGNED_BYTE, (ByteBuffer) null);
            CHECK_GL();
            textureEntry.isSRGB = true;
        }
        CHECK_GL();
    }

    private void drawGeometry(long geometryId, long indicesCount, long indicesOffset, UltralightGPUState state) {
        CHECK_GL();
        glfwMakeContextCurrent(context.getActiveWindow());

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
        if (state.enableScissor) {
            glEnable(GL_SCISSOR_TEST);
            IntRect r = state.scissorRect;
            glScissor(r.getLeft(), r.getTop(), (r.getRight() - r.getLeft()), (r.getBottom() - r.getTop()));
        } else {
            glDisable(GL_SCISSOR_TEST);
        }

        if (state.enableBlend) {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }
        else
            glDisable(GL_BLEND);
        CHECK_GL();
        glDrawElements(GL_TRIANGLES, Math.toIntExact(indicesCount), GL_UNSIGNED_INT, indicesOffset * Integer.BYTES);
        CHECK_GL();
        glBindVertexArray(0);

        if (context.isEnableOffscreenGl()) {
            RenderBufferEntry renderBufferEntry = renderBufferMap.computeIfAbsent(state.renderBufferId, unused -> new RenderBufferEntry());

            if (renderBufferEntry.bitmap != null)
                renderBufferEntry.needsUpdate = true;
        }

        CHECK_GL();
    }

    public void bindTexture(int texture_unit, long texture_id) {
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        bindUltralightTexture(texture_id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        CHECK_GL();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        CHECK_GL();
    }

    public void bindUltralightTexture(long textureId) {
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
        GeometryEntry geometryEntry = geometryMap.get(geometryId);
        if (geometryEntry == null) {
            throw new RuntimeException("Geometry ID doesn't exist.");
        }

        Long vaoEntry = geometryEntry.vaoMap.get(glfwGetCurrentContext());
        if (vaoEntry != null) {
            return;
        }

        vaoEntry = (long) glGenVertexArrays();
        glBindVertexArray(Math.toIntExact(vaoEntry));

        glBindBuffer(GL_ARRAY_BUFFER, geometryEntry.vboVertices);
        CHECK_GL();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometryEntry.vboIndices);
        CHECK_GL();

        if (geometryEntry.vertexFormat == UltralightVertexBufferFormat.FORMAT_2F_4UB_2F_2F_28F) {
            int stride = 140;

            glVertexAttribPointer(0, 2, GL_FLOAT, false, stride, 0);
            glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, true, stride, 8);
            glVertexAttribPointer(2, 2, GL_FLOAT, false, stride, 12);
            glVertexAttribPointer(3, 2, GL_FLOAT, false, stride, 20);
            glVertexAttribPointer(4, 4, GL_FLOAT, false, stride, 28);
            glVertexAttribPointer(5, 4, GL_FLOAT, false, stride, 44);
            glVertexAttribPointer(6, 4, GL_FLOAT, false, stride, 60);
            glVertexAttribPointer(7, 4, GL_FLOAT, false, stride, 76);
            glVertexAttribPointer(8, 4, GL_FLOAT, false, stride, 92);
            glVertexAttribPointer(9, 4, GL_FLOAT, false, stride, 108);
            glVertexAttribPointer(10, 4, GL_FLOAT, false, stride, 124);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);
            glEnableVertexAttribArray(3);
            glEnableVertexAttribArray(4);
            glEnableVertexAttribArray(5);
            glEnableVertexAttribArray(6);
            glEnableVertexAttribArray(7);
            glEnableVertexAttribArray(8);
            glEnableVertexAttribArray(9);
            glEnableVertexAttribArray(10);

            CHECK_GL();
        } else if (geometryEntry.vertexFormat == UltralightVertexBufferFormat.FORMAT_2F_4UB_2F) {
            int stride = 20;

            glVertexAttribPointer(0, 2, GL_FLOAT, false, stride, 0);
            glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, true, stride, 8);
            glVertexAttribPointer(2, 2, GL_FLOAT, false, stride, 12);

            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            glEnableVertexAttribArray(2);

            CHECK_GL();
        } else {
            throw new RuntimeException("Unhandled vertex format: " + geometryEntry.vertexFormat);
        }

        glBindVertexArray(0);

        geometryEntry.vaoMap.put(glfwGetCurrentContext(), vaoEntry);
        CHECK_GL();
    }

    void updateUniforms(UltralightGPUState state) {
        boolean flip_y = state.renderBufferId != 0;
        UltralightMatrix modelViewProjection = applyProjection(state.transformMatrix, state.viewportWidth, state.viewportHeight, flip_y);

        float[] params = new float[]{
                (float) (glfwGetTime() / 1000.0), state.viewportWidth, state.viewportHeight, 1.0f
        };
        setUniform4f("State", params);
        CHECK_GL();
        UltralightMatrix4x4 mat = modelViewProjection.getMatrix4x4();
        setUniformMatrix4fv("Transform", mat.getData());
        CHECK_GL();
        setUniform4fv("Scalar4", state.uniformScalar);
        CHECK_GL();
        float[] vectorData = new float[8 * 4];

        for (int i = 0; i < 8 * 4; i += 4) {
            System.arraycopy(state.uniformVector[i / 4].getValue(), 0, vectorData, i, 4);
        }

        setUniform4fv("Vector", vectorData);
        CHECK_GL();
        setUniform1ui("ClipSize", state.clipSize);
        CHECK_GL();

        float[] clip = new float[8 * 16];
        for (int i = 0; i < 8 * 16; i += 16) {
            System.arraycopy(state.clip[i / 16].getData(), 0, clip, i, 16);
        }

        setUniformMatrix4fv("Clip", clip);
        CHECK_GL();
    }

    private void setUniform4fv(String name, float[] val) {
        glUniform4fv(glGetUniformLocation(Math.toIntExact(curProgramId), name), val);
    }

    private void setUniform1ui(String name, int val) {
        glUniform1ui(glGetUniformLocation(Math.toIntExact(curProgramId), name), val);
    }

    private void setUniform4f(String name, float[] params) {
        glUniform4fv(glGetUniformLocation(Math.toIntExact(curProgramId), name), params);
    }

    UltralightMatrix applyProjection(UltralightMatrix4x4 transform, float screenWidth, float screenHeight, boolean flipY) {
        UltralightMatrix transform_mat = new UltralightMatrix();
        transform_mat.set(transform);

        UltralightMatrix result = new UltralightMatrix();
        result.setOrthographicProjection(screenWidth, screenHeight, flipY);
        result.transform(transform_mat);

        return result;
    }

    void setUniformMatrix4fv(String name, float[] val) {
        glUniformMatrix4fv(glGetUniformLocation(Math.toIntExact(curProgramId), name), false, val);
    }

    private void loadPrograms() {
        loadProgram(KSHADERTYPE_FILL);
        loadProgram(KSHADERTYPE_FILLPATH);
    }

    void selectProgram(short type) {
        CHECK_GL();
        ProgramEntry programEntry = this.programs.get(type);
        if (programEntry == null) {
            throw new RuntimeException("Missing shader type: " + type);
        }
        curProgramId = programEntry.programId;
        glUseProgram(programEntry.programId);
        CHECK_GL();
    }


    private void loadProgram(short type) {
        CHECK_GL();
        ProgramEntry prog = new ProgramEntry();
        if (type == KSHADERTYPE_FILL) {
            prog.vertShaderId = loadShaderFromSource(GL_VERTEX_SHADER,
                    readShader("shader_v2f_c4f_t2f_t2f_d28f.vs"), "shader_v2f_c4f_t2f_t2f_d28f.vert");
            prog.fragShaderId = loadShaderFromSource(GL_FRAGMENT_SHADER,
                    readShader("shader_fill_frag.fs"), "shader_fill.frag");
        } else if (type == KSHADERTYPE_FILLPATH) {
            prog.vertShaderId = loadShaderFromSource(GL_VERTEX_SHADER,
                    readShader("shader_v2f_c4f_t2f.vs"), "shader_v2f_c4f_t2f.vert");
            prog.fragShaderId = loadShaderFromSource(GL_FRAGMENT_SHADER,
                    readShader("shader_fill_path_frag.fs"), "shader_fill_path.frag");
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
        CHECK_GL();

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
        entry.texId = glGenTextures();
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
        int error;
        if ((error = GL11.glGetError()) != GL11.GL_NO_ERROR) {
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
        entry.texId = glGenTextures();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, (int) entry.texId);

        // Allocate texture in linear space.
        // We will convert back to sRGB for monitor when binding renderbuffer 0
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int) entry.width, (int) entry.height, 0,
                GL_BGRA, GL_UNSIGNED_BYTE, (ByteBuffer) null);

        if (context.msaaEnabled()) {
            // Allocate the multisampled texture
            entry.msaaTexId = glGenTextures();
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
        CHECK_GL();
        glActiveTexture(GL_TEXTURE0);
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
        entry.texId = 0;
        CHECK_GL();
        if (entry.msaaTexId != 0L) {
            glDeleteTextures(Math.toIntExact(entry.msaaTexId));
            entry.msaaTexId = 0;
        }
        CHECK_GL();
    }

    @Override
    public void createRenderBuffer(long renderBufferId, UltralightRenderBuffer buffer) {
        CHECK_GL();

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
        CHECK_GL();
    }

    @Override
    public void destroyRenderBuffer(long renderBufferId) {
        CHECK_GL();
        if (renderBufferId == 0)
            return;

        long previous_context = glfwGetCurrentContext();

        RenderBufferEntry renderBufferEntry = renderBufferMap.get(renderBufferId);
        for (Map.Entry<Long, FBOEntry> entry : renderBufferEntry.fboMap.entrySet()) {

            long context = entry.getKey();
            FBOEntry fboEntry = entry.getValue();
            glfwMakeContextCurrent(context);
            glDeleteFramebuffers(Math.toIntExact(fboEntry.fboId));
            fboEntry.fboId = 0;
            CHECK_GL();
            if (this.context.msaaEnabled()) {
                glDeleteFramebuffers(Math.toIntExact(fboEntry.msaaFboId));
                fboEntry.msaaFboId = 0;
            }
            CHECK_GL();
        }

        if (context.isEnableOffscreenGl()) {

            // Clean up PBOs if a bitmap is bound
            if (renderBufferEntry.bitmap != null) {
                glDeleteBuffers(Math.toIntExact(renderBufferEntry.pboId));
                renderBufferEntry.pboId = 0;
            }

        }
        CHECK_GL();
        renderBufferMap.remove(renderBufferId);

        glfwMakeContextCurrent(previous_context);
        CHECK_GL();
    }

    @Override
    public void createGeometry(long geometryId, UltralightVertexBuffer vertices, UltralightIndexBuffer indices) {
        CHECK_GL();
        GeometryEntry geometry = new GeometryEntry();
        geometry.vertexFormat = vertices.format;

        geometry.vboVertices = glGenBuffers();

        glBindBuffer(GL_ARRAY_BUFFER, geometry.vboVertices);
        glBufferData(GL_ARRAY_BUFFER, vertices.data, GL_DYNAMIC_DRAW);
        CHECK_GL();

        geometry.vboIndices = glGenBuffers();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.vboIndices);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.data,
                GL_STATIC_DRAW);

        geometryMap.put(geometryId, geometry);
        CHECK_GL();
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
    }

    @Override
    public void destroyGeometry(long geometryId) {
        CHECK_GL();
        GeometryEntry geometry = geometryMap.get(geometryId);
        glDeleteBuffers(geometry.vboIndices);
        glDeleteBuffers(geometry.vboVertices);
        geometry.vboIndices = 0;
        geometry.vboVertices = 0;

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
        CHECK_GL();
    }

    @Override
    public void updateCommandList(UltralightCommandList list) {
        CHECK_GL();
        if (list.data.length != 0) {
            for (UltralightCommand datum : list.data) {
                this.commands.add(datum);
            }
        }
        CHECK_GL();
    }
}
