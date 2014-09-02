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

in vec3 pos_eye;
in vec3 n_eye;
uniform samplerCube cube_texture;
uniform mat4 view; // view matrix
out vec4 frag_colour;

void main () {
  /* reflect ray around normal from eye to surface */
  vec3 incident_eye = normalize (pos_eye);
  vec3 normal = normalize (n_eye);

  vec3 reflected = reflect (incident_eye, normal);
  // convert from eye to world space
  reflected = vec3 (inverse (view) * vec4 (reflected, 0.0));

  frag_colour = texture (cube_texture, reflected);
}


/*#version 420 core
layout (binding = 0) uniform samplerCube tex_cubemap;
in VS_OUT
{
  vec3 normal;
  vec3 view;
} fs_in;

out vec4 color;

void main(void){
  // Reflect view vector about the plane defined by the normal
  // at the fragment
  vec3 r = reflect(fs_in.view, normalize(fs_in.normal));
  // Sample from scaled using reflection vector
  color = texture(tex_cubemap, r);
}
*/
