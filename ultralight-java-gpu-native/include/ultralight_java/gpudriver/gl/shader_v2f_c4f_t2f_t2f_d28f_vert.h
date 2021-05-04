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

#include <string>

static std::string shader_v2f_c4f_t2f_t2f_d28f_vert() {
  return R"(#version 150

// Program Uniforms
uniform vec4 State;
uniform mat4 Transform;
uniform vec4 Scalar4[2];
uniform vec4 Vector[8];
uniform uint ClipSize;
uniform mat4 Clip[8];

// Uniform Accessor Functions
float Time() { return State[0]; }
float ScreenWidth() { return State[1]; }
float ScreenHeight() { return State[2]; }
float ScreenScale() { return State[3]; }
float Scalar(uint i) { if (i < 4u) return Scalar4[0][i]; else return Scalar4[1][i - 4u]; }
vec4 sRGBToLinear(vec4 val) {
    return val;
    // return vec4(val.xyz * (val.xyz * (val.xyz * 0.305306011 + 0.682171111) + 0.012522878), val.w);
}
// Vertex Attributes
in vec2 in_Position;
in vec4 in_Color;
in vec2 in_TexCoord;
in vec2 in_ObjCoord;
in vec4 in_Data0;
in vec4 in_Data1;
in vec4 in_Data2;
in vec4 in_Data3;
in vec4 in_Data4;
in vec4 in_Data5;
in vec4 in_Data6;

// Out Params
out vec4 ex_Color;
out vec2 ex_TexCoord;
out vec4 ex_Data0;
out vec4 ex_Data1;
out vec4 ex_Data2;
out vec4 ex_Data3;
out vec4 ex_Data4;
out vec4 ex_Data5;
out vec4 ex_Data6;
out vec2 ex_ObjectCoord;
out vec2 ex_ScreenCoord;

void main(void)
{
  ex_ObjectCoord = in_ObjCoord;
  gl_Position = Transform * vec4(in_Position, 0.0, 1.0);
  ex_Color = in_Color;
  ex_TexCoord = in_TexCoord;
  ex_Data0 = in_Data0;
  ex_Data1 = in_Data1;
  ex_Data2 = in_Data2;
  ex_Data3 = in_Data3;
  ex_Data4 = in_Data4;
  ex_Data5 = in_Data5;
  ex_Data6 = in_Data6;
}

)";
}