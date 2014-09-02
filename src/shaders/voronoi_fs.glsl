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

#ifdef GL_ES
precision highp float;
#endif
#define SITE_MAX 50
varying vec2 vTexCoord1;
uniform int numberSites;
uniform float p;
uniform float width;
uniform float height;
uniform bool weighted;
uniform vec3 sites[SITE_MAX];
uniform float ws[SITE_MAX];
uniform vec3 siteColors[SITE_MAX];
uniform mat4 modelMat;

#define R 200.0
#define PI 3.1415926535897

out vec4 frag_color;

vec4 sample(float x0, float y0) {
  float minDist = -1.0; 
  float dist;
  vec4 color;
  float x = (x0 - width * 0.5) / R;
  float y = (y0 - height * 0.5) / R;
  float z = 1.0 - x * x - y * y;
  if (z < 0.0) {
    color = vec4(0,0,0,1);
  } else {
    z = sqrt(z);
    vec3 v = vec3(x, y, z);
    float il = clamp(dot(v, vec3(1,1,2)) / sqrt(6.) * 0.7 + dot(v, vec3(0,0,1)) * 0.03, 0., 1.) + 0.3;
    color = vec4(il, il, il, 1.0);
    for (int i = 0; i < SITE_MAX; i++) {
      if (i < numberSites) {
        vec3 vs = (modelMat * vec4(sites[i], 1)).xyz;
        float w = weighted ? abs(ws[i]) : 1.0;
        if (z > 0.0) {
          float d = dot(vs,v);
          float dist = acos(d) / PI * 180.0;
          if (dist < 1.0) {
            il = 1.0;
            color = vec4(il, il, il,1);
            break;
          } else if (minDist < 0. || minDist > dist / w) {
            color = vec4(siteColors[i] * il, 1.0);
            minDist = dist / w;
          }
        }
      } else {
        break;
      }
    }
  }
  return color;
}

void main(void) {
  float x = vTexCoord1.x * width, y = vTexCoord1.y * height;
  //gl_FragColor = sample(x, y);
  frag_color = sample(x, y);
}
