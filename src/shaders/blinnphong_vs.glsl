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

// Per-vertex inputs
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

mat4 model = mat4(1.0, 0.0, 0.0, 0.0, 
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0, 
                      0.0, 0.0, 0.0, 1.0);

uniform mat4 proj, view, mvMatrix;

// Inputs from vertex shader
out VS_OUT
{
    vec3 N;
    vec3 L;
    vec3 V;
    vec3 H;
} vs_out;

// Position of light
uniform vec3 light_pos = vec3(10.0, 10.0, 10.0);

void main(void){
    // Calculate view-space coordinate
  vec4 P =  mvMatrix * vec4(position, 1.0);

  // Calculate normal in view-space
  vs_out.N =  mat3(mvMatrix) * normal;
  //vs_out.N = mat3(view) * normal;
  
  // Calculate light vector
  vs_out.L = light_pos - position.xyz;
  
  // Calculate view vector
  vs_out.V = -P.xyz;

  vs_out.H = normalize(vs_out.L + vs_out.V);

  // Calculate the clip-space position of each vertex
  gl_Position = proj * P;
}
