/***************************************************************************
*
* Copyright 2014 Bruno Ordozgoiti
* Distributed under GNU GPLv3
*
* This file is part of GLandbox.
*
* GLandbox is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* GLandbox is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with GLandbox. If not, see <http://www.gnu.org/licenses/>.
*
***************************************************************************/

#version 420 core
layout (binding = 0) uniform sampler2D tex_envmap;
in VS_OUT{
  vec3 normal;
  vec3 view;
} fs_in;
out vec4 color;
void main(void){
  // u will be our normalized view vector
  vec3 u = normalize(fs_in.view);
  // Reflect u about the plane defined by the normal at the fragment
  vec3 r = reflect(u, normalize(fs_in.normal));
  // Compute texture coordinate from reflection vector
  vec2 tc;
  tc.y = r.y; r.y = 0.0;
  tc.x = normalize(r).x * 0.5;
  // Scale and bias texture coordinate based on direction
  // of reflection vector
  float s = sign(r.z) * 0.5;
  tc.s = 0.75 - s * (0.5 - tc.s);
  tc.t = 0.5 + 0.5 * tc.t;
  // Sample from scaled and biased texture coordinate
  color = texture(tex_envmap, tc);
}
