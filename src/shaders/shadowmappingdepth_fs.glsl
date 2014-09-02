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

#version 420

layout (binding = 0) uniform sampler2D tex_depth;

layout (location = 0) out vec4 color;

void main(void){
  float d = texelFetch(tex_depth, ivec2(gl_FragCoord.xy * 3.0) + ivec2(850, 1050), 0).r;
  d = (d - 0.95) * 15.0;
  color = vec4(d);
  vec4 a = texelFetch(tex_depth, ivec2(gl_FragCoord.xy), 0);
  //  color = a;
}
