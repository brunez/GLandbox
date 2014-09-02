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


//TODO Study shared layout
//Global scale to apply to everything
//Translation in X, Y, and Z
//Rotation around X, Y, and Z axes
//A generalized projection matrix to apply
//after scale and rotate

// "offset" is an input vertex attribute
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

//TODO Careful! Compiler might wipe out uniforms if they are not used
layout(std140, binding = 2) uniform TransformBlock{
    float scale;
    vec3 translation;
    float rotation[3];
    mat4 projection_matrix;
} transform;

uniform mat4 proj, view;

// Declare VS_OUT as an output interface block
out VS_OUT{
    vec3 color;
} vs_out;

void main(void){
      vs_out.color = vec3(0.0, position.y*0.8, 0.41);
      //float a = transform.scale;
      gl_Position = proj * view * vec4 (position, 1.0);      
}
