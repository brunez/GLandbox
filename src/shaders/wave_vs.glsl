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

layout (location = 0) in vec3 position;

uniform float step;
uniform mat4 proj, view, mvMatrix;

// Declare VS_OUT as an output interface block
out VS_OUT{
    vec3 color;
    vec3 pos;  
    float step; 
} vs_out;

void main(void){
      vs_out.color = vec3(0.0, position.y*0.8, 0.41);
      vs_out.pos = position;
      vs_out.step = step;
      gl_Position = proj * mvMatrix * vec4 (position, 1.0);
      gl_Position.y += sin(position.x/7.0+position.z/3.0+step)/2.0;     
}
