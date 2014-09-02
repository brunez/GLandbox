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

//interface block. Match is done using block name, not instance name
in VS_OUT{
  vec3 pos;
  vec4 color;
} fs_in;

out vec4 frag_color;

void main () {
  float color;  
  if(mod(fs_in.pos.z, 2.0) > 1.0){
    color = step(1.0, mod(fs_in.pos.x, 2.0)) -0.3;
  } else {
    color = step(1.0, mod(fs_in.pos.x+1.0, 2.0)) -0.3;
  }
  
  frag_color = vec4(vec3(color), 1.0);
};
