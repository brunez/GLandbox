# Copyright 2013 Douglas Linder
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required (VERSION 2.8)
project (OpenGL4_Tutorials)

# Debug mode?
set(CMAKE_BUILD_TYPE Release)

# We have configure style deps, sadly.
include(ExternalProject)

# Code type
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11 -g -O0 -pthread -I/usr/include/freetype2 -I/usr/include/freetype2/freetype") #Include both freetypes to make sure it compiles with older versions

# Opengl
#include(FindOpenGL)

# Includes
include_directories(${EXTRA_INCLUDES})
include_directories(src)
include_directories(src/log)
include_directories(src/shaders)
include_directories(src/gland)
include_directories(src/gland/controllers)
include_directories(deps)
include_directories(deps/unlicensed)


set(COMMON_LIBS 
  -lfreetype
  -lglfw3  
  -lGLU 
  -lGL 
  -lGLEW 
  -lX11 -lXrandr -lXi -lXxf86vm
  -lpng
  )

# finds OpenGL, GLU and X11
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
  message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
#set(GL_LIBRARY GL GLU X11)

# Collect source files
file(GLOB_RECURSE SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/gland/*.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/log/*.cpp 
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/*.cc 
  ${CMAKE_CURRENT_SOURCE_DIR}/deps/*.cpp
  )


set(EXAMPLES
  cubes
  sea
  light_models
  bump_mapping
  environment_maps
  cube_maps
  shadow_mapping
  fog
)

foreach(EXAMPLE ${EXAMPLES})

  set(BIN ${EXAMPLE})

  add_executable(${EXAMPLE}
    src/examples/${EXAMPLE}/${EXAMPLE}.cpp
    ${SOURCES}
    )

  target_link_libraries(${EXAMPLE} ${COMMON_LIBS})

endforeach(EXAMPLE)

