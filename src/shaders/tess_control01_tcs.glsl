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

layout (vertices = 3) out;

void main(void){
     
     if (gl_InvocationID == 0){
          gl_TessLevelInner[0] = 5.0;
          gl_TessLevelOuter[0] = 5.0;
          gl_TessLevelOuter[1] = 5.0;
          gl_TessLevelOuter[2] = 5.0;
     }
     
     gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}
