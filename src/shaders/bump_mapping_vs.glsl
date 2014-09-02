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

#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 tangent;
// Although the model file used in this example includes
// a bitangent, we're not using it and will calculate our
// own from the normal and tangent.
// layout (location = 3) in vec3 bitangent;
layout (location = 4) in vec2 texcoord;

out VS_OUT{
  vec2 texcoord;
  vec3 eyeDir;
  vec3 lightDir;
  vec3 normal;
} vs_out;

uniform float step;
uniform mat4 proj, view, mvMatrix;
uniform vec3 light_pos = vec3(10.0, 10.0, 10.0);

void main(void){

  // Calculate vertex position in view space.
  vec4 P = mvMatrix * vec4(position, 1.0);
  
  // Calculate normal (N) and tangent (T) vectors in view space from
  // incoming object space vectors.
  //vec3 V = P.xyz;
  vec3 V = position.xyz;
  //vec3 N = normalize(mat3(view) * normal);
    
  mat4 normalMatrix = transpose(inverse(mvMatrix));
  //vs_out.normal = vec3(normalMatrix * vec4(normal, 1.0));

  vs_out.normal = vec3(sin(step*position.y*position.y*1), sin(step*position.x*position.x*1), 1.0);

  /*
  //    vec3 T = normalize(mat3(view) * tangent);
  vec3 T = normalize(tangent);
  // Calculate the bitangent vector (B) from the normal and tangent
  // vectors.
  vec3 B = cross(N, T);
  */

  // The light vector (L) is the vector from the point of interest to
  // the light. Calculate that and multiply it by the TBN matrix.
  vs_out.lightDir = mat3(view)*light_pos - P.xyz;  
  //vs_out.lightDir = normalize(vec3(dot(L, T), dot(L, B), dot(L, N)));

  // The view vector is the vector from the point of interest to the
  // viewer, which in view space is simply the negative of the position.
  // Calculate that and multiply it by the TBN matrix.
  vs_out.eyeDir = -P.xyz;

  //vs_out.eyeDir = normalize(vec3(dot(V, T), dot(V, B), dot(V, N)));

  // Pass the texture coordinate through unmodified so that the fragment
  // shader can fetch from the normal and color maps.
  vs_out.texcoord = texcoord;

  // Pass the per-vertex normal so that the fragment shader can
  // turn the normal map on and off.
  //vs_out.normal = N;

  // Calculate clip coordinates by multiplying our view position by
  // the projection matrix.
  gl_Position = proj * P;
}
