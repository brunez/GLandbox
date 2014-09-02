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

#version 410 core

out vec4 color;

// Color and normal maps
layout (binding = 0) uniform sampler2D tex_color;
layout (binding = 1) uniform sampler2D tex_normal;

in VS_OUT
{
    vec2 texcoord;
    vec3 eyeDir;
    vec3 lightDir;
    vec3 normal;
} fs_in;

void main(void)
{
    // Normalize our incomming view and light direction vectors.
    vec3 V = normalize(fs_in.eyeDir);
    vec3 L = normalize(fs_in.lightDir);
    // Read the normal from the normal map and normalize it.
    //vec3 N = normalize(texture(tex_normal, fs_in.texcoord).rgb * 2.0 - vec3(1.0));
    // Uncomment this to use surface normals rather than the normal map   
    vec3 N = fs_in.normal;
    //N = vec3(0.0, 0.0, 1.0);
    // Calculate R ready for use in Phong lighting.
    vec3 R = reflect(-L, N);
    float dotRV;
    if(dot(-L, N) >= 0){
      dotRV = 0.0;
    } else {
      dotRV = dot(R, V);
    }

    // Fetch the diffuse albedo from the texture.
    //vec3 diffuse_albedo = texture(tex_color, fs_in.texcoord).rgb;
    vec3 diffuse_albedo = vec3(0.5);
    // Calculate diffuse color with simple N dot L.
    vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo;
    // Uncomment this to turn off diffuse shading
    // diffuse = vec3(0.0);

    // Assume that specular albedo is white - it could also come from a texture
    vec3 specular_albedo = vec3(1.0);
    // Calculate Phong specular highlight
    vec3 specular = max(pow(dotRV, 20.0), 0.0) * specular_albedo;
    // Uncomment this to turn off specular highlights
    //specular = vec3(0.0);

    // Final color is diffuse + specular
    color = vec4(diffuse + specular, 1.0);
}
