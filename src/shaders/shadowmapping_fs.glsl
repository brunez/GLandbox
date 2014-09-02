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

layout (location = 0) out vec4 color;

layout (binding = 0) uniform sampler2DShadow shadow_tex;

in VS_OUT{
    vec3 normal;
    vec3 lightPos;
    vec3 viewVector;
    mat4 view;
    vec4 shadowCoord;
} fsIn;

// Provisionally hard-coded
uniform vec3 diffuse_albedo = vec3(0.2, 0.2, 0.2);
uniform vec3 specular_albedo = vec3(1.0);
uniform float specular_power = 128.0;
uniform vec3 ambient = vec3(0.0, 0.0, 0.0);
uniform bool full_shading = true;

void main(void){
    vec3 normal = normalize(fsIn.normal);
    vec3 lightPos = normalize(fsIn.lightPos);
    vec3 viewVector = normalize(fsIn.viewVector);

    // Calculate reflected ray locally
    vec3 reflected;
    float dotRV;
    if(dot(-lightPos, normal) >= 0){
      dotRV = 0.0;
    } else {
      reflected = reflect(-lightPos, normal);
      dotRV = dot(reflected, viewVector);
    }

    vec3 diffuse = max(dot(normal, lightPos), 0.0) * diffuse_albedo;
    vec3 specular = pow(max(dotRV, 0.0), specular_power) * specular_albedo;

    color = textureProj(shadow_tex, fsIn.shadowCoord) * mix(vec4(1.0), vec4(diffuse + specular, 1.0), bvec4(full_shading));

}
