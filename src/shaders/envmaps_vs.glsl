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

uniform mat4 mvMatrix, view, proj;

layout (location = 0) in vec4 position;
layout (location = 1) in vec3 normal;

out VS_OUT{
  vec3 normal;
  vec3 view;
} vs_out;

void main(void){
  vec4 pos_vs = mvMatrix * position;
  vs_out.normal = mat3(mvMatrix) * normal;
  vs_out.view = pos_vs.xyz;
  gl_Position = proj * pos_vs;
}
