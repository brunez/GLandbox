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

// "offset" is an input vertex attribute

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

uniform mat4 proj, view;

// Declare VS_OUT as an output interface block
out VS_OUT{
    vec4 color;
} vs_out;

void main(void){
     const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0),
           vec4(-0.25, -0.25, 0.5, 1.0),
           vec4( 0.25, 0.0, 0.5, 1.0));

      // Add "offset" to our hard-coded vertex position
      //gl_Position = proj * view * vertices[gl_VertexID];
      gl_Position = proj * view * position;
      

      // Output a fixed value for vs_color
      //vs_out.color = color;
      //vs_out.color = vec4(0.0, 0.8, 1.0, 1.0);

}
