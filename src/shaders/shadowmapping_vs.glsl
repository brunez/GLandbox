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

uniform mat4 proj, view, mvMatrix, normalMatrix, shadowMatrix;

// Inputs from vertex shader
out VS_OUT{
    vec3 normal;
    vec3 lightPos;
    vec3 viewVector;
    vec4 shadowCoord;
} vsOut;

// Position of light in view space
uniform vec3 viewLightPos;

void main(void){
  // Calculate view-space coordinate
  vec4 mvPos = mvMatrix * vec4(position, 1.0);
  vsOut.normal = vec3(normalMatrix * vec4(normal, 1.0)); 
  vsOut.lightPos = viewLightPos - vec3(mvPos);
  vsOut.viewVector = -mvPos.xyz;
  vsOut.shadowCoord = shadowMatrix * vec4(position, 1.0);
  gl_Position = proj * mvPos;
}
