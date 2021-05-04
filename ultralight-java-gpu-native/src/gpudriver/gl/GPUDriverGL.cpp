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

#include "ultralight_java/gpudriver/gl/GPUDriverGL.h"
#include "ultralight_java/gpudriver/gl/GPUContextGL.h"
#include <Ultralight/platform/Platform.h>
#include <Ultralight/platform/FileSystem.h>
#include <iostream>
#include <fstream>
#include <sstream>
#if ENABLE_OFFSCREEN_GL
#include "shader_fill_frag.h"
#include "shader_fill_path_frag.h"
#include "shader_v2f_c4f_t2f_t2f_d28f_vert.h"
#include "shader_v2f_c4f_t2f_vert.h"
#else
#include "ultralight_java/gpudriver/gl/shader_fill_frag.h"
#include "ultralight_java/gpudriver/gl/shader_fill_path_frag.h"
#include "ultralight_java/gpudriver/gl/shader_v2f_c4f_t2f_t2f_d28f_vert.h"
#include "ultralight_java/gpudriver/gl/shader_v2f_c4f_t2f_vert.h"
#endif

#define SHADER_PATH "glsl/"

#ifdef _DEBUG
#if _WIN32
#define INFO(x) { std::cerr << "[INFO] " << __FUNCSIG__ << " @ Line " << __LINE__ << ":\n\t" << x << std::endl; }
#else
#define INFO(x) { std::cerr << "[INFO] " << __PRETTY_FUNCTION__ << " @ Line " << __LINE__ << ":\n\t" << x << std::endl; }
#endif
#else
#define INFO(x)
#endif

#if _WIN32
#include <Windows.h>
#define FATAL(x) { std::stringstream str; \
  str << "[ERROR] " << __FUNCSIG__ << " @ Line " << __LINE__ << ":\n\t" << x << std::endl; \
  OutputDebugStringA(str.str().c_str()); \
  std::cerr << str.str() << std::endl; \
  __debugbreak(); std::cin.get(); exit(-1); }
#else
#define FATAL(x) { std::cerr << "[ERROR] " << __PRETTY_FUNCTION__ << " @ Line " << __LINE__ << ":\n\t" << x << std::endl; \
  std::cin.get(); exit(-1); }
#endif

static void ReadFile(const char* filepath, std::string& result) {
  // To maintain predictable behavior across platforms we use
  // whatever FileSystem that Ultralight is using:
  ultralight::FileSystem* fs = ultralight::Platform::instance().file_system();
  if (!fs)
  FATAL("No FileSystem defined.");

  auto handle = fs->OpenFile(filepath, false);
  if (handle == ultralight::invalidFileHandle)
  FATAL("Could not open file path: " << filepath);

  int64_t fileSize = 0;
  fs->GetFileSize(handle, fileSize);
  result.resize((size_t)fileSize);
  fs->ReadFromFile(handle, &result[0], fileSize);
  fs->CloseFile(handle);
}

inline char const* glErrorString(GLenum const err) noexcept
{
  switch (err)
  {
    // OpenGL 2.0+ Errors:
    case GL_NO_ERROR: return "GL_NO_ERROR";
    case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
    case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW";
    case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW";
    case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
      // OpenGL 3.0+ Errors
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
    default: return "UNKNOWN ERROR";
  }
}

inline std::string GetShaderLog(GLuint shader_id) {
  GLint length, result;
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
  std::string str(length, ' ');
  glGetShaderInfoLog(shader_id, (GLsizei)str.length(), &result, &str[0]);
  return str;
}

inline std::string GetProgramLog(GLuint program_id) {
  GLint length, result;
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &length);
  std::string str(length, ' ');
  glGetProgramInfoLog(program_id, (GLsizei)str.length(), &result, &str[0]);
  return str;
}

static GLuint LoadShaderFromSource(GLenum shader_type, const char* source, const char* filename) {
  GLint compileStatus;
  GLuint shader_id = glCreateShader(shader_type);
  glShaderSource(shader_id, 1, &source, NULL);
  glCompileShader(shader_id);
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compileStatus);
  if (compileStatus == GL_FALSE)
  FATAL("Unable to compile shader. Filename: " << filename << "\n\tError:"
                                               << glErrorString(glGetError()) << "\n\tLog: " << GetShaderLog(shader_id))
  return shader_id;
}

static GLuint LoadShaderFromFile(GLenum shader_type, const char* filename) {
  std::string shader_source;
  std::string path = std::string(SHADER_PATH) + filename;
  ReadFile(path.c_str(), shader_source);
  GLint compileStatus;
  const char* shader_source_str = shader_source.c_str();
  GLuint shader_id = glCreateShader(shader_type);
  glShaderSource(shader_id, 1, &shader_source_str, NULL);
  glCompileShader(shader_id);
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &compileStatus);
  if (compileStatus == GL_FALSE)
  FATAL("Unable to compile shader. Filename: " << filename << "\n\tError:"
                                               << glErrorString(glGetError()) << "\n\tLog: " << GetShaderLog(shader_id))
  return shader_id;
}

#ifdef _DEBUG
#define CHECK_GL()  {if (GLenum err = glGetError()) FATAL(glErrorString(err)) }
#else
#define CHECK_GL()
#endif

namespace ultralight {

GPUDriverGL::GPUDriverGL(GPUContextGL* context) {
  context_ = context;
}

#if ENABLE_OFFSCREEN_GL
void GPUDriverGL::SetRenderBufferBitmap(uint32_t render_buffer_id,
  RefPtr<Bitmap> bitmap) {
  // Get our entry from RenderBuffer map, creating it if it does not exist
  auto& entry = render_buffer_map[render_buffer_id];

  CHECK_GL();

  // Delete any existing PBOs
  if (entry.bitmap)
    glDeleteBuffers(1, &entry.pbo_id);

  entry.bitmap = bitmap;

  if (entry.bitmap) {
    // Generate PBO ids
    glGenBuffers(1, &entry.pbo_id);

    // Setup PBOs
    glBindBuffer(GL_PIXEL_PACK_BUFFER, entry.pbo_id);
    glBufferData(GL_PIXEL_PACK_BUFFER, bitmap->size(), 0, GL_STREAM_READ);

    CHECK_GL();

    // Reset glBindBuffer
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    CHECK_GL();
  }

  // Setup rest of RenderBufferEntry, we have to be careful not to overwrite
  // the FBO id which may or may not be already set in CreateRenderBuffer()
  entry.is_bitmap_dirty = false;
  entry.is_first_draw = true;
  entry.needs_update = false;

  if (entry.texture_id) {
    // We already have a backing texture
    MakeTextureSRGBIfNeeded(entry.texture_id);
  }
}

bool GPUDriverGL::IsRenderBufferBitmapDirty(uint32_t render_buffer_id) {
  auto& entry = render_buffer_map[render_buffer_id];
  return entry.is_bitmap_dirty;
}

void GPUDriverGL::SetRenderBufferBitmapDirty(uint32_t render_buffer_id,
  bool dirty) {
  auto& entry = render_buffer_map[render_buffer_id];
  entry.is_bitmap_dirty = dirty;
}
#endif

void GPUDriverGL::CreateTexture(uint32_t texture_id,
                                Ref<Bitmap> bitmap) {
  if (bitmap->IsEmpty()) {
    CreateFBOTexture(texture_id, bitmap);
    return;
  }

  CHECK_GL();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  CHECK_GL();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  CHECK_GL();

  TextureEntry& entry = texture_map[texture_id];
  glGenTextures(1, &entry.tex_id);
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, entry.tex_id);
  CHECK_GL();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap->row_bytes() / bitmap->bpp());
  CHECK_GL();
  if (bitmap->format() == kBitmapFormat_A8_UNORM) {
    const void* pixels = bitmap->LockPixels();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, bitmap->width(), bitmap->height(), 0,
                 GL_RED, GL_UNSIGNED_BYTE, pixels);
    bitmap->UnlockPixels();
  } else if (bitmap->format() == kBitmapFormat_BGRA8_UNORM_SRGB) {
    const void* pixels = bitmap->LockPixels();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmap->width(), bitmap->height(), 0,
                 GL_BGRA, GL_UNSIGNED_BYTE, pixels);
    bitmap->UnlockPixels();
  } else {
    FATAL("Unhandled texture format: " << bitmap->format())
  }

  CHECK_GL();
  glGenerateMipmap(GL_TEXTURE_2D);
  CHECK_GL();
}

void GPUDriverGL::UpdateTexture(uint32_t texture_id,
                                Ref<Bitmap> bitmap) {
  glActiveTexture(GL_TEXTURE0 + 0);
  TextureEntry& entry = texture_map[texture_id];
  glBindTexture(GL_TEXTURE_2D, entry.tex_id);
  CHECK_GL();
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, bitmap->row_bytes() / bitmap->bpp());

  if (!bitmap->IsEmpty()) {
    if (bitmap->format() == kBitmapFormat_A8_UNORM) {
      const void* pixels = bitmap->LockPixels();
      glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, bitmap->width(), bitmap->height(), 0,
                   GL_RED, GL_UNSIGNED_BYTE, pixels);
      bitmap->UnlockPixels();
    } else if (bitmap->format() == kBitmapFormat_BGRA8_UNORM_SRGB) {
      const void* pixels = bitmap->LockPixels();
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, bitmap->width(), bitmap->height(), 0,
                   GL_BGRA, GL_UNSIGNED_BYTE, pixels);
      bitmap->UnlockPixels();
    } else {
      FATAL("Unhandled texture format: " << bitmap->format());
    }

    CHECK_GL();
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  CHECK_GL();
}

void GPUDriverGL::BindTexture(uint8_t texture_unit, uint32_t texture_id) {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  BindUltralightTexture(texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  CHECK_GL();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  CHECK_GL();
}

void GPUDriverGL::DestroyTexture(uint32_t texture_id) {
  TextureEntry& entry = texture_map[texture_id];
  glDeleteTextures(1, &entry.tex_id);
  CHECK_GL();
  if (entry.msaa_tex_id)
    glDeleteTextures(1, &entry.msaa_tex_id);
  CHECK_GL();
}

void GPUDriverGL::CreateRenderBuffer(uint32_t render_buffer_id,
                                     const RenderBuffer& buffer) {
  CHECK_GL();

  if (render_buffer_id == 0) {
    INFO("Should not be reached! Render Buffer ID 0 is reserved for default framebuffer.");
    return;
  }

  RenderBufferEntry& entry = render_buffer_map[render_buffer_id];
  entry.texture_id = buffer.texture_id;

  TextureEntry& textureEntry = texture_map[buffer.texture_id];
  textureEntry.render_buffer_id = render_buffer_id;

  // We don't actually create FBOs here-- they are lazily-created
  // for each active window during BindRenderBuffer (this is because
  // FBOs are not shared between contexts in GL 3.2)
  CHECK_GL();
}

void GPUDriverGL::BindRenderBuffer(uint32_t render_buffer_id) {
  if (render_buffer_id == 0) {
    // Render buffer id '0' is reserved for window's backbuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return;
  }

  CreateFBOIfNeededForActiveContext(render_buffer_id);

  RenderBufferEntry& entry = render_buffer_map[render_buffer_id];

  auto i = entry.fbo_map.find(context_->active_window());
  if (i == entry.fbo_map.end())
    return;

  auto& fbo_entry = i->second;

  if (context_->msaa_enabled()) {
    // We use the MSAA FBO when doing multisampled rendering.
    // The other FBO (entry.fbo_id) is used for resolving.
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_entry.msaa_fbo_id);
    fbo_entry.needs_resolve = true;
  } else {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_entry.fbo_id);
  }

  CHECK_GL();
}

void GPUDriverGL::ClearRenderBuffer(uint32_t render_buffer_id) {
  //glfwMakeContextCurrent(context_->active_window());

  BindRenderBuffer(render_buffer_id);
  glDisable(GL_SCISSOR_TEST);
  CHECK_GL();
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  CHECK_GL();
  glClear(GL_COLOR_BUFFER_BIT);
  CHECK_GL();
}

void GPUDriverGL::DestroyRenderBuffer(uint32_t render_buffer_id) {
  if (render_buffer_id == 0)
    return;

//  auto previous_context = glfwGetCurrentContext();

  RenderBufferEntry& entry = render_buffer_map[render_buffer_id];
  for (auto i = entry.fbo_map.begin(); i != entry.fbo_map.end(); ++i) {
//    auto context = i->first;
    auto fbo_entry = i->second;
//    glfwMakeContextCurrent(context);
    glDeleteFramebuffers(1, &fbo_entry.fbo_id);
    CHECK_GL();
    if (context_->msaa_enabled())
      glDeleteFramebuffers(1, &fbo_entry.msaa_fbo_id);
    CHECK_GL();
  }

#if ENABLE_OFFSCREEN_GL
  // Clean up PBOs if a bitmap is bound
  if (entry.bitmap)
    glDeleteBuffers(1, &entry.pbo_id);
#endif
  CHECK_GL();
  render_buffer_map.erase(render_buffer_id);

//  glfwMakeContextCurrent(previous_context);
}

void GPUDriverGL::CreateGeometry(uint32_t geometry_id,
                                 const VertexBuffer& vertices,
                                 const IndexBuffer& indices) {

  GeometryEntry geometry;
  geometry.vertex_format = vertices.format;

  glGenBuffers(1, &geometry.vbo_vertices);
  glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, vertices.size, vertices.data, GL_DYNAMIC_DRAW);
  CHECK_GL();

  glGenBuffers(1, &geometry.vbo_indices);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.vbo_indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size, indices.data,
               GL_STATIC_DRAW);
  CHECK_GL();

  geometry_map[geometry_id] = geometry;
}

void GPUDriverGL::UpdateGeometry(uint32_t geometry_id,
                                 const VertexBuffer& vertices,
                                 const IndexBuffer& indices) {

  GeometryEntry& geometry = geometry_map[geometry_id];
  CHECK_GL();
  glBindBuffer(GL_ARRAY_BUFFER, geometry.vbo_vertices);
  glBufferData(GL_ARRAY_BUFFER, vertices.size, vertices.data, GL_DYNAMIC_DRAW);
  CHECK_GL();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry.vbo_indices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size, indices.data, GL_STATIC_DRAW);
  CHECK_GL();
  CHECK_GL();

}

void GPUDriverGL::DrawGeometry(uint32_t geometry_id,
                               uint32_t indices_count,
                               uint32_t indices_offset,
                               const GPUState& state) {

//  glfwMakeContextCurrent(context_->active_window());

  if (programs_.empty())
    LoadPrograms();

  BindRenderBuffer(state.render_buffer_id);

  SetViewport(state.viewport_width, state.viewport_height);

  GeometryEntry& geometry = geometry_map[geometry_id];
  SelectProgram((ProgramType)state.shader_type);
  UpdateUniforms(state);

  CHECK_GL();

  CreateVAOIfNeededForActiveContext(geometry_id);
  auto vao_entry = geometry.vao_map[context_->active_window()];
  glBindVertexArray(vao_entry);
  CHECK_GL();

  BindTexture(0, state.texture_1_id);
  BindTexture(1, state.texture_2_id);
  BindTexture(2, state.texture_3_id);

  CHECK_GL();

  if (state.enable_scissor) {
    glEnable(GL_SCISSOR_TEST);
    const IntRect& r = state.scissor_rect;
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
                 (GLvoid*)(indices_offset * sizeof(unsigned int)));
  CHECK_GL();
  glBindVertexArray(0);

#if ENABLE_OFFSCREEN_GL
  auto& rbuf = render_buffer_map[state.render_buffer_id];
  if (rbuf.bitmap)
    rbuf.needs_update = true;
#endif

  batch_count_++;

  CHECK_GL();
}

void GPUDriverGL::DestroyGeometry(uint32_t geometry_id) {
  GeometryEntry& geometry = geometry_map[geometry_id];
  CHECK_GL();
  glDeleteBuffers(1, &geometry.vbo_indices);
  glDeleteBuffers(1, &geometry.vbo_vertices);
  CHECK_GL();

//  auto previous_context = glfwGetCurrentContext();

  for (auto i = geometry.vao_map.begin(); i != geometry.vao_map.end(); ++i) {
//    auto context = i->first;
    auto vao_entry = i->second;
//    glfwMakeContextCurrent(context);
    glDeleteVertexArrays(1, &vao_entry);
    CHECK_GL();
  }

  CHECK_GL();
  geometry_map.erase(geometry_id);

//  glfwMakeContextCurrent(previous_context);
}

void GPUDriverGL::DrawCommandList() {
  if (command_list_.empty())
    return;

  int boundFrameBuffer = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFrameBuffer);

//  glfwMakeContextCurrent(context_->active_window());

  CHECK_GL();

  batch_count_ = 0;

  glEnable(GL_BLEND);
  glDisable(GL_SCISSOR_TEST);
  glDisable(GL_DEPTH_TEST);
  glDepthFunc(GL_NEVER);
  glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

  CHECK_GL();

  for (auto i = command_list_.begin(); i != command_list_.end(); ++i) {
    switch (i->command_type) {
      case kCommandType_DrawGeometry:
        DrawGeometry(i->geometry_id, i->indices_count, i->indices_offset, i->gpu_state);
        break;
      case kCommandType_ClearRenderBuffer:
        ClearRenderBuffer(i->gpu_state.render_buffer_id);
        break;
    };
  }

  command_list_.clear();

  glDisable(GL_SCISSOR_TEST);

#if ENABLE_OFFSCREEN_GL
  GLenum format = Platform::instance().config().use_bgra_for_offscreen_rendering ?
    GL_BGRA : GL_RGBA;

  for (auto i = render_buffer_map.begin(); i != render_buffer_map.end(); ++i) {
    auto& rbuf = i->second;

    if (rbuf.bitmap && rbuf.needs_update) {
      ResolveIfNeeded(i->first);
      glBindFramebuffer(GL_FRAMEBUFFER, i->second.fbo_id);
      CHECK_GL();
      // Perform blocking copy of pixels from FBO to current PBO
      glBindBuffer(GL_PIXEL_PACK_BUFFER, rbuf.pbo_id);
      CHECK_GL();
      glReadPixels(0, 0, rbuf.bitmap->width(), rbuf.bitmap->height(), format, GL_UNSIGNED_BYTE, 0);
      CHECK_GL();
      UpdateBitmap(rbuf, rbuf.pbo_id);
      rbuf.needs_update = false;
  }
}
#endif
  glBindFramebuffer(GL_FRAMEBUFFER, boundFrameBuffer);

  CHECK_GL();
}

void GPUDriverGL::BindUltralightTexture(uint32_t ultralight_texture_id) {
  TextureEntry& entry = texture_map[ultralight_texture_id];
  ResolveIfNeeded(entry.render_buffer_id);
  glBindTexture(GL_TEXTURE_2D, entry.tex_id);
  CHECK_GL();
}

void GPUDriverGL::LoadPrograms(void) {
  LoadProgram(ultralight::kShaderType_Fill);
  LoadProgram(ultralight::kShaderType_FillPath);
}

void GPUDriverGL::DestroyPrograms(void) {
  GLenum ErrorCheckValue = glGetError();
  glUseProgram(0);
  for (auto i = programs_.begin(); i != programs_.end(); i++) {
    ProgramEntry& prog = i->second;
    glDetachShader(prog.program_id, prog.vert_shader_id);
    glDetachShader(prog.program_id, prog.frag_shader_id);
    glDeleteShader(prog.vert_shader_id);
    glDeleteShader(prog.frag_shader_id);
    glDeleteProgram(prog.program_id);
  }
  programs_.clear();
}

void GPUDriverGL::LoadProgram(ProgramType type) {
  GLenum ErrorCheckValue = glGetError();
  ProgramEntry prog;
  if (type == kShaderType_Fill)
  {
    prog.vert_shader_id = LoadShaderFromSource(GL_VERTEX_SHADER,
                                               shader_v2f_c4f_t2f_t2f_d28f_vert().c_str(), "shader_v2f_c4f_t2f_t2f_d28f.vert");
    prog.frag_shader_id = LoadShaderFromSource(GL_FRAGMENT_SHADER,
                                               shader_fill_frag().c_str(), "shader_fill.frag");
  }
  else if (type == kShaderType_FillPath) {
    prog.vert_shader_id = LoadShaderFromSource(GL_VERTEX_SHADER,
                                               shader_v2f_c4f_t2f_vert().c_str(), "shader_v2f_c4f_t2f.vert");
    prog.frag_shader_id = LoadShaderFromSource(GL_FRAGMENT_SHADER,
                                               shader_fill_path_frag().c_str(), "shader_fill_path.frag");
  }

  prog.program_id = glCreateProgram();
  glAttachShader(prog.program_id, prog.vert_shader_id);
  glAttachShader(prog.program_id, prog.frag_shader_id);

  glBindAttribLocation(prog.program_id, 0, "in_Position");
  glBindAttribLocation(prog.program_id, 1, "in_Color");
  glBindAttribLocation(prog.program_id, 2, "in_TexCoord");

  if (type == kShaderType_Fill) {
    glBindAttribLocation(prog.program_id, 3, "in_ObjCoord");
    glBindAttribLocation(prog.program_id, 4, "in_Data0");
    glBindAttribLocation(prog.program_id, 5, "in_Data1");
    glBindAttribLocation(prog.program_id, 6, "in_Data2");
    glBindAttribLocation(prog.program_id, 7, "in_Data3");
    glBindAttribLocation(prog.program_id, 8, "in_Data4");
    glBindAttribLocation(prog.program_id, 9, "in_Data5");
    glBindAttribLocation(prog.program_id, 10, "in_Data6");
  }

  glLinkProgram(prog.program_id);
  glUseProgram(prog.program_id);

  if (type == kShaderType_Fill) {
    glUniform1i(glGetUniformLocation(prog.program_id, "Texture1"), 0);
    glUniform1i(glGetUniformLocation(prog.program_id, "Texture2"), 1);
    glUniform1i(glGetUniformLocation(prog.program_id, "Texture3"), 2);
  }

  if (glGetError())
  FATAL("Unable to link shader.\n\tError:" << glErrorString(glGetError()) << "\n\tLog: " << GetProgramLog(prog.program_id))

  programs_[type] = prog;
}

void GPUDriverGL::SelectProgram(ProgramType type) {
  auto i = programs_.find(type);
  if (i != programs_.end()) {
    cur_program_id_ = i->second.program_id;
    glUseProgram(i->second.program_id);
  } else {
    FATAL("Missing shader type: " << type);
  }
}

void GPUDriverGL::UpdateUniforms(const GPUState& state) {
  bool flip_y = state.render_buffer_id != 0;
  Matrix model_view_projection = ApplyProjection(state.transform, (float)state.viewport_width, (float)state.viewport_height, flip_y);

  float params[4] = { (float)(getTime() / 1000.0), (float)state.viewport_width, (float)state.viewport_height, 1.0f };
  SetUniform4f("State", params);
  CHECK_GL();
  ultralight::Matrix4x4 mat = model_view_projection.GetMatrix4x4();
  SetUniformMatrix4fv("Transform", 1, mat.data);
  CHECK_GL();
  SetUniform4fv("Scalar4", 2, &state.uniform_scalar[0]);
  CHECK_GL();
  SetUniform4fv("Vector", 8, &state.uniform_vector[0].value[0]);
  CHECK_GL();
  SetUniform1ui("ClipSize", state.clip_size);
  CHECK_GL();
  SetUniformMatrix4fv("Clip", 8, &state.clip[0].data[0]);
  CHECK_GL();
}

void GPUDriverGL::SetUniform1ui(const char* name, GLuint val) {
  glUniform1ui(glGetUniformLocation(cur_program_id_, name), val);
}

void GPUDriverGL::SetUniform1f(const char* name, float val) {
  glUniform1f(glGetUniformLocation(cur_program_id_, name), (GLfloat)val);
}

void GPUDriverGL::SetUniform1fv(const char* name, size_t count, const float* val) {
  glUniform1fv(glGetUniformLocation(cur_program_id_, name), (GLsizei)count, val);
}

void GPUDriverGL::SetUniform4f(const char* name, const float val[4]) {
  glUniform4f(glGetUniformLocation(cur_program_id_, name),
              (GLfloat)val[0], (GLfloat)val[1], (GLfloat)val[2], (GLfloat)val[3]);
}

void GPUDriverGL::SetUniform4fv(const char* name, size_t count, const float* val) {
  glUniform4fv(glGetUniformLocation(cur_program_id_, name), (GLsizei)count, val);
}

void GPUDriverGL::SetUniformMatrix4fv(const char* name, size_t count, const float* val) {
  glUniformMatrix4fv(glGetUniformLocation(cur_program_id_, name), (GLsizei)count, false, val);
}

void GPUDriverGL::SetViewport(uint32_t width, uint32_t height) {
  glViewport(0, 0, static_cast<GLsizei>(width),
             static_cast<GLsizei>(height));
}

Matrix GPUDriverGL::ApplyProjection(const Matrix4x4& transform, float screen_width, float screen_height, bool flip_y) {
  Matrix transform_mat;
  transform_mat.Set(transform);

  Matrix result;
  result.SetOrthographicProjection(screen_width, screen_height, flip_y);
  result.Transform(transform_mat);

  return result;
}

void GPUDriverGL::CreateFBOTexture(uint32_t texture_id, Ref<Bitmap> bitmap) {
  CHECK_GL();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  CHECK_GL();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  CHECK_GL();

  TextureEntry& entry = texture_map[texture_id];
  entry.width = bitmap->width();
  entry.height = bitmap->height();

  // Allocate a single-sampled texture
  glGenTextures(1, &entry.tex_id);
  glActiveTexture(GL_TEXTURE0 + 0);
  glBindTexture(GL_TEXTURE_2D, entry.tex_id);

  // Allocate texture in linear space.
  // We will convert back to sRGB for monitor when binding renderbuffer 0
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, entry.width, entry.height, 0,
               GL_BGRA, GL_UNSIGNED_BYTE, nullptr);

  if (context_->msaa_enabled()) {
    // Allocate the multisampled texture
    glGenTextures(1, &entry.msaa_tex_id);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, entry.msaa_tex_id);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, entry.width, entry.height, false);
  }

  CHECK_GL();
  glGenerateMipmap(GL_TEXTURE_2D);
  CHECK_GL();
}

void GPUDriverGL::CreateFBOIfNeededForActiveContext(uint32_t render_buffer_id) {
  if (render_buffer_id == 0)
    return;

  auto i = render_buffer_map.find(render_buffer_id);
  if (i == render_buffer_map.end()) {
    FATAL("Error, render buffer entry should exist here.")
    return;
  }

  RenderBufferEntry& entry = i->second;
  auto j = entry.fbo_map.find(context_->active_window());
  if (j != entry.fbo_map.end())
    return; // Already exists, we can return

  FBOEntry& fbo_entry = entry.fbo_map[context_->active_window()];

  glGenFramebuffers(1, &fbo_entry.fbo_id);
  CHECK_GL();
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_entry.fbo_id);
  CHECK_GL();

  TextureEntry& textureEntry = texture_map[entry.texture_id];

#if ENABLE_OFFSCREEN_GL
  if (entry.bitmap)
    MakeTextureSRGBIfNeeded(entry.texture_id);
#endif

  glBindTexture(GL_TEXTURE_2D, textureEntry.tex_id);
  CHECK_GL();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureEntry.tex_id, 0);
  CHECK_GL();

  GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, drawBuffers);
  CHECK_GL();

  GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (result != GL_FRAMEBUFFER_COMPLETE)
  FATAL("Error creating FBO, this usually fails if your DPI scale is invalid or View dimensions are massive: " << result);
  CHECK_GL();

  if (!context_->msaa_enabled()) {
    return;
  }

  // Create MSAA FBO
  glGenFramebuffers(1, &fbo_entry.msaa_fbo_id);
  CHECK_GL();
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_entry.msaa_fbo_id);
  CHECK_GL();

  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureEntry.msaa_tex_id);
  CHECK_GL();
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureEntry.msaa_tex_id, 0);
  CHECK_GL();

  glDrawBuffers(1, drawBuffers);
  CHECK_GL();

  result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  if (result != GL_FRAMEBUFFER_COMPLETE)
  FATAL("Error creating MSAA FBO, this usually fails if your DPI scale is invalid or View dimensions are massive: " << result);
  CHECK_GL();
}

void GPUDriverGL::CreateVAOIfNeededForActiveContext(uint32_t geometry_id) {
  auto i = geometry_map.find(geometry_id);
  if (i == geometry_map.end()) {
    FATAL("Geometry ID doesn't exist.");
    return;
  }

  auto& geometry_entry = i->second;

  auto j = geometry_entry.vao_map.find(context_->active_window());
  if (j != geometry_entry.vao_map.end())
    return; // Already exists, we can return

  GLuint vao_entry;

  glGenVertexArrays(1, &vao_entry);
  glBindVertexArray(vao_entry);

  glBindBuffer(GL_ARRAY_BUFFER, geometry_entry.vbo_vertices);
  CHECK_GL();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry_entry.vbo_indices);
  CHECK_GL();

  if (geometry_entry.vertex_format == kVertexBufferFormat_2f_4ub_2f_2f_28f) {
    GLsizei stride = 140;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid*)8);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)12);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)20);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)28);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)44);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)60);
    glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)76);
    glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)92);
    glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)108);
    glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, stride, (GLvoid*)124);

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
  } else if (geometry_entry.vertex_format == kVertexBufferFormat_2f_4ub_2f) {
    GLsizei stride = 20;

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)0);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid*)8);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (GLvoid*)12);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    CHECK_GL();
  } else {
    FATAL("Unhandled vertex format: " << geometry_entry.vertex_format);
  }

  glBindVertexArray(0);

  geometry_entry.vao_map[context_->active_window()] = vao_entry;
}

void GPUDriverGL::ResolveIfNeeded(uint32_t render_buffer_id) {
  if (!context_->msaa_enabled())
    return;

  if (render_buffer_id == 0)
    return;

  RenderBufferEntry& renderBufferEntry = render_buffer_map[render_buffer_id];
  if (!renderBufferEntry.texture_id)
    return;

  auto i = renderBufferEntry.fbo_map.find(context_->active_window());
  if (i == renderBufferEntry.fbo_map.end())
    return;

  FBOEntry& fbo_entry = i->second;

  TextureEntry& textureEntry = texture_map[renderBufferEntry.texture_id];
  if (fbo_entry.needs_resolve) {
    GLint drawFboId = 0, readFboId = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);
    glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &readFboId);
    CHECK_GL();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_entry.fbo_id);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo_entry.msaa_fbo_id);
    CHECK_GL();
    glBlitFramebuffer(0, 0, textureEntry.width, textureEntry.height, 0, 0,
                      textureEntry.width, textureEntry.height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    CHECK_GL();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, drawFboId);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, readFboId);
    CHECK_GL();
    fbo_entry.needs_resolve = false;
  }
}

void GPUDriverGL::MakeTextureSRGBIfNeeded(uint32_t texture_id) {
  TextureEntry& textureEntry = texture_map[texture_id];
  if (!textureEntry.is_sRGB) {
    // We need to make the primary texture sRGB
    // First, Destroy existing texture.
    glDeleteTextures(1, &textureEntry.tex_id);
    CHECK_GL();
    // Create new sRGB texture
    glGenTextures(1, &textureEntry.tex_id);
    glActiveTexture(GL_TEXTURE0 + 0);
    glBindTexture(GL_TEXTURE_2D, textureEntry.tex_id);
    CHECK_GL();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, textureEntry.width, textureEntry.height, 0,
                 GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
    CHECK_GL();
    textureEntry.is_sRGB = true;
  }
}

#if ENABLE_OFFSCREEN_GL
void GPUDriverGL::UpdateBitmap(RenderBufferEntry& entry, GLuint pbo_id) {
  // Map previous PBO to system memory and copy it to bitmap (may block)
  CHECK_GL();
  glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_id);
  CHECK_GL();
  GLubyte* src = (GLubyte*)glMapBuffer(GL_PIXEL_PACK_BUFFER, GL_READ_ONLY);
  CHECK_GL();
  void* dest = entry.bitmap->LockPixels();
  memcpy(dest, src, entry.bitmap->size());
  entry.bitmap->UnlockPixels();
  glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
  CHECK_GL();
  // Flag our bitmap as dirty
  entry.is_bitmap_dirty = true;
}
#endif

}  // namespace ultralight