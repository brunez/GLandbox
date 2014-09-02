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

#version 430 core

// Per-vertex inputs
layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

mat4 model = mat4(30.0, 0.0, 0.0, 0.0, 
                      0.0, 3.0, 0.0, 0.0,
                  0.0, 0.0, 3.0, 0.0, 
                      0.0, 0.0, 0.0, 1.0);

uniform mat4 proj, view;

// Light and material properties
uniform vec3 light_pos = vec3(100.0, 100.0, 100.0);
uniform vec3 diffuse_albedo = vec3(0.5, 0.2, 0.7);
uniform vec3 specular_albedo = vec3(0.7);
uniform float specular_power = 128.0;
uniform vec3 ambient = vec3(0.1, 0.1, 0.1);

// Outputs to the fragment shader
out VS_OUT{
  vec3 color;
} vs_out;

void main(void){

  // Calculate view-space coordinate
  vec4 P = view * position;
  // Calculate normal in view space
  vec3 N =   normal;

  // Calculate view-space light vector
  vec3 L = light_pos - P.xyz;
  // Calculate view vector (simply the negative of the
  // view-space position)
  vec3 V = -P.xyz;

  // Normalize all three vectors
  N = normalize(N);
  L = normalize(L);
  V = normalize(V);

  // Calculate R by reflecting -L around the plane defined by N
  vec3 R = reflect(-L, N);

  // Calculate the diffuse and specular contributions
  vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
  vec3 specular = pow(max(dot(R, V), 0.0), specular_power) *
    specular_albedo;

  // Send the color output to the fragment shader
  vs_out.color = ambient + diffuse + specular;

  // Calculate the clip-space position of each vertex
  gl_Position = proj * P;
}
