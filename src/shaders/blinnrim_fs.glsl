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

// Output
layout (location = 0) out vec4 color;

// Input from vertex shader
in VS_OUT{
    vec3 N;
    vec3 L;
    vec3 V;
    vec3 H;
    mat4 view;
} fs_in;

// Material properties
uniform vec3 diffuse_albedo = vec3(0.5, 0.2, 0.7);
uniform vec3 specular_albedo = vec3(0.7);
uniform float specular_power = 128.0;
uniform vec3 ambient = vec3(0.1, 0.1, 0.1);

// Uniforms controlling the rim light effect
uniform vec3 rim_color = vec3(0.4);
uniform float rim_power = 16.0;

vec3 calculate_rim(vec3 N, vec3 V){
  // Calculate the rim factor
  float f = 1.0 - dot(N, V);
  // Constrain it to the range 0 to 1 using a smooth step function
  f = smoothstep(0.0, 1.0, f);
  // Raise it to the rim exponent
  f = pow(f, rim_power);
  // Finally, multiply it by the rim color
  return f * rim_color;
}

void main(void){
    // Normalize the incoming N, L and V vectors
    vec3 N = normalize(fs_in.N);
    vec3 L = normalize(fs_in.L);
    vec3 V = normalize(fs_in.V);

    // Calculate the half vector, H
    //vec3 H = normalize(L + V);
    vec3 H = normalize(fs_in.H);

    // Compute the diffuse and specular components for each fragment
    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
    vec3 specular = pow(max(dot(N, H), 0.0), specular_power) * specular_albedo;

    vec3 rim = calculate_rim(mat3(fs_in.view)*N, V);

    // Write final color to the framebuffer
    color = vec4(ambient + diffuse + specular + rim, 1.0);
}
