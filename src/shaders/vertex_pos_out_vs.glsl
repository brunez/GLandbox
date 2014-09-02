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

// "vs_color" is an output that will be sent to the next shader stage
out vec4 vs_color;

void main(void){

      gl_Position = proj * view * position;      

     // Output a fixed value for vs_color
     //vs_color = vec4(1.0);
     vs_color = color;   
}
