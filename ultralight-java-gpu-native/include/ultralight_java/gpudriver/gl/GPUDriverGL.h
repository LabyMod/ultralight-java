/*
 * Ultralight Java - Java wrapper for the Ultralight web engine
 * Copyright (C) 2021 LabyMedia and contributors
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

#pragma once
#include <Ultralight/platform/GPUDriver.h>

#include <chrono>
#include <map>
#include <vector>

#include "../common/GPUDriverImpl.h"
#include "GPUContextGL.h"
#include "glad.h"

inline float start_time =
    std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch())
        .count();
inline float getTime() {
  return std::chrono::duration_cast<std::chrono::milliseconds>(
             std::chrono::system_clock::now().time_since_epoch())
             .count() -
         start_time;
}

namespace ultralight {

typedef ShaderType ProgramType;

class GPUDriverGL : public GPUDriverImpl {
 public:
  GPUDriverGL(GPUContextGL* context);

  virtual ~GPUDriverGL() {}

  virtual GPUContextGL* GetContext() { return context_; }

  virtual const char* name() override { return "OpenGL"; }

  virtual void BeginDrawing() override {}

  virtual void EndDrawing() override {}

#if ENABLE_OFFSCREEN_GL
  virtual void SetRenderBufferBitmap(uint32_t render_buffer_id,
                                     RefPtr<Bitmap> bitmap);

  virtual bool IsRenderBufferBitmapDirty(uint32_t render_buffer_id);

  virtual void SetRenderBufferBitmapDirty(uint32_t render_buffer_id,
                                          bool dirty);
#endif

  virtual void CreateTexture(uint32_t texture_id, Ref<Bitmap> bitmap) override;

  virtual void UpdateTexture(uint32_t texture_id, Ref<Bitmap> bitmap) override;

  virtual void BindTexture(uint8_t texture_unit, uint32_t texture_id) override;

  virtual void DestroyTexture(uint32_t texture_id) override;

  virtual void CreateRenderBuffer(uint32_t render_buffer_id,
                                  const RenderBuffer& buffer) override;

  virtual void BindRenderBuffer(uint32_t render_buffer_id) override;

  virtual void ClearRenderBuffer(uint32_t render_buffer_id) override;

  virtual void DestroyRenderBuffer(uint32_t render_buffer_id) override;

  virtual void CreateGeometry(uint32_t geometry_id,
                              const VertexBuffer& vertices,
                              const IndexBuffer& indices) override;

  virtual void UpdateGeometry(uint32_t geometry_id,
                              const VertexBuffer& vertices,
                              const IndexBuffer& indices) override;

  virtual void DrawGeometry(uint32_t geometry_id, uint32_t indices_count,
                            uint32_t indices_offset,
                            const GPUState& state) override;

  virtual void DestroyGeometry(uint32_t geometry_id) override;

  virtual void DrawCommandList() override;

  void BindUltralightTexture(uint32_t ultralight_texture_id);

  void LoadPrograms();
  void DestroyPrograms();

  void LoadProgram(ProgramType type);
  void SelectProgram(ProgramType type);
  void UpdateUniforms(const GPUState& state);
  void SetUniform1ui(const char* name, GLuint val);
  void SetUniform1f(const char* name, float val);
  void SetUniform1fv(const char* name, size_t count, const float* val);
  void SetUniform4f(const char* name, const float val[4]);
  void SetUniform4fv(const char* name, size_t count, const float* val);
  void SetUniformMatrix4fv(const char* name, size_t count, const float* val);
  void SetViewport(uint32_t width, uint32_t height);

 protected:
  Matrix ApplyProjection(const Matrix4x4& transform, float screen_width,
                         float screen_height, bool flip_y);

  void CreateFBOTexture(uint32_t texture_id, Ref<Bitmap> bitmap);

  struct TextureEntry {
    GLuint tex_id = 0;       // GL Texture ID
    GLuint msaa_tex_id = 0;  // GL Texture ID (only used if MSAA is enabled)
    uint32_t render_buffer_id =
        0;                 // Used to check if we need to perform MSAA resolve
    GLuint width, height;  // Used when resolving MSAA FBO, only valid if FBO
    bool is_sRGB = false;  // Whether or not the primary texture is sRGB or not.
  };

  // Maps Ultralight Texture IDs to OpenGL texture handles
  std::map<uint32_t, TextureEntry> texture_map;

  struct GeometryEntry {
    // VAOs are not shared across GL contexts so we create them lazily for each
    std::map<void*, GLuint> vao_map;
    VertexBufferFormat vertex_format;
    GLuint vbo_vertices = 0;  // VBO id for vertices
    GLuint vbo_indices = 0;   // VBO id for indices
  };
  std::map<uint32_t, GeometryEntry> geometry_map;

  struct FBOEntry {
    GLuint fbo_id =
        0;  // GL FBO ID (if MSAA is enabled, this will be used for resolve)
    GLuint msaa_fbo_id = 0;  // GL FBO ID for MSAA
    bool needs_resolve =
        false;  // Whether or not we need to perform MSAA resolve
  };

  struct RenderBufferEntry {
    // FBOs are not shared across GL contexts so we create them lazily for each
    std::map<void*, FBOEntry> fbo_map;
    uint32_t texture_id =
        0;  // The Ultralight texture ID backing this RenderBuffer.
#if ENABLE_OFFSCREEN_GL
    RefPtr<Bitmap> bitmap;
    GLuint pbo_id = 0;
    bool is_bitmap_dirty = false;
    bool is_first_draw = true;
    bool needs_update = false;
#endif
  };

  void CreateFBOIfNeededForActiveContext(uint32_t render_buffer_id);

  void CreateVAOIfNeededForActiveContext(uint32_t geometry_id);

  void ResolveIfNeeded(uint32_t render_buffer_id);

  void MakeTextureSRGBIfNeeded(uint32_t texture_id);

#if ENABLE_OFFSCREEN_GL
  void UpdateBitmap(RenderBufferEntry& entry, GLuint pbo_id);
#endif

  std::map<uint32_t, RenderBufferEntry> render_buffer_map;

  struct ProgramEntry {
    GLuint program_id;
    GLuint vert_shader_id;
    GLuint frag_shader_id;
  };
  std::map<ProgramType, ProgramEntry> programs_;
  GLuint cur_program_id_;

  GPUContextGL* context_;
};

}  // namespace ultralight