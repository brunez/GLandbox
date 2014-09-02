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
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

uniform mat4 proj, view;

// Declare VS_OUT as an output interface block
out VS_OUT{
    vec4 color;
} vs_out;

void main(void){
      // Output a fixed value for vs_color
      //vs_out.color = color;              
  vs_out.color = vec4(0.2, 0.2, 0.2, 1.0);              

      gl_Position = proj * view * vec4 (position, 1.0);      
      //gl_Position = vec4 (1.0, 1.0, 1.0, 1.0);      
}
