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
    vec3 pos;  
    float step; 
} vs_out;


vec3 mod289(vec3 x){
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}
vec2 mod289(vec2 x){
  return x - floor(x * (1.0 / 289.0)) * 289.0;
}
vec3 permute(vec3 x){
  return mod289(((x*34.0)+1.0)*x);
}
float snoise(vec2 v){
  const vec4 C = vec4(0.211324865405187, // (3.0-sqrt(3.0))/6.0
                      0.366025403784439, // 0.5*(sqrt(3.0)-1.0)
                      -0.577350269189626, // -1.0 + 2.0 * C.x
                      0.024390243902439); // 1.0 / 41.0
  // First corner
  vec2 i = floor(v + dot(v, C.yy) );
  vec2 x0 = v - i + dot(i, C.xx);
  // Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = (x0.x > x0.y) ? vec2(1.0, 0.0) : vec2(0.0, 1.0);
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = x0.xyxy + C.xxzz;
  x12.xy -= i1;
  // Permutations
  i = mod289(i); // Avoid truncation effects in permutation
  vec3 p = permute( permute( i.y + vec3(0.0, i1.y, 1.0 ))
                    + i.x + vec3(0.0, i1.x, 1.0 ));
  vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x12.xy,x12.xy), dot(x12.zw,x12.zw)), 0.0);
  m = m*m ;
  m = m*m ;
  // Gradients: 41 points uniformly over a line, mapped onto a diamond.
  // The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)
  vec3 x = 2.0 * fract(p * C.www) - 1.0;
  vec3 h = abs(x) - 0.5;
  vec3 ox = floor(x + 0.5);
  vec3 a0 = x - ox;
  // Normalise gradients implicitly by scaling m
  // Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159 - 0.85373472095314 * ( a0*a0 + h*h );
  // Compute final noise value at P
  vec3 g;
  g.x = a0.x * x0.x + h.x * x0.y;
  g.yz = a0.yz * x12.xz + h.yz * x12.yw;
  return 130.0 * dot(m, g);
}


void main(void){
      vs_out.color = vec3(0.0, position.y*0.8, 0.41);
      vs_out.pos = position;
      vs_out.step = step;
      gl_Position = proj * view * vec4 (position, 1.0);
      gl_Position.y += sin(position.x/7.0+position.z/3.0+step)/2.0;      
      float n = snoise(vec2(position.x/5.0, position.z/5.0+step/10.0));
 n += 0.5 * snoise(vec2(2.0) - vec2(0.0, step*0.1));
  n += 0.25 * snoise(vec2(4.0) - vec2(0.0,  step*0.1));
  n += 0.125 * snoise(vec2(8.0) - vec2(0.0,  step*0.1));
  n += 0.0625 * snoise(vec2(16.0) - vec2(0.0,  step/5.0));
  n += 0.03125 * snoise(vec2(32.0) - vec2(0.0, step/5.0));
  n = n * 0.1;

  gl_Position.y += n;

}
