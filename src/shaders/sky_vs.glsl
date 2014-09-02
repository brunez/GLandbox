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

//layout (location = 0) in vec3 position;

out VS_OUT{
  vec3 tc;
} vs_out;

uniform mat4 view;

void main(void){
  vec3[4] vertices = vec3[4](vec3(-1.0, -1.0, 1.0),
                             vec3( 1.0, -1.0, 1.0),
                             vec3(-1.0, 1.0, 1.0),
                             vec3( 1.0, 1.0, 1.0));
  vec3[4] co = vec3[4](vec3(-1.0, 1.0, 1.0),
                       vec3( 1.0, 1.0, 1.0),
                       vec3(-1.0, -1.0, 1.0),
                       vec3( 1.0, -1.0, 1.0));
  
  vs_out.tc = mat3(view) * co[gl_VertexID];
  gl_Position = vec4(vertices[gl_VertexID], 1.0);
}
