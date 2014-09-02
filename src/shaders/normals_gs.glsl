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

#version 330
layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in Data
{
    vec4 position;
    vec4 normal;
    vec4 color;
    mat4 mvp;
} vdata[3];

out Data
{
    vec4 color;
} gdata;

void main()
{
    const vec4 green = vec4(0.0f, 1.0f, 0.0f, 1.0f);
    const vec4 blue = vec4(0.0f, 0.0f, 1.0f, 1.0f);

    for (int i = 0; i < 3; i++)
    {
        gl_Position = vdata[i].mvp * vdata[i].position;
        gdata.color = green;
        EmitVertex();

        gl_Position = vdata[i].mvp * (vdata[i].position + vdata[i].normal);
        gdata.color = blue;
        EmitVertex();

        EndPrimitive();
    }
}
