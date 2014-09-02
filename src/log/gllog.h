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

#ifndef LOG_H
#define LOG_H

#include <fstream>
#include <stdio.h>
#include <GLFW/glfw3.h>

namespace gllog{
  bool restart_gl_log();
  bool gl_log (const char* message, const char* filename, int line);
  void glfw_error_callback (int error, const char* description);
  void log_gl_params();
  void update_fps_counter (GLFWwindow* window);
  void _print_shader_info_log(unsigned int shaderIndex);
  void _print_programme_info_log (unsigned int programIndex);
  void print_all ();
  const char* GL_type_to_string (unsigned int type);
  bool isValid(unsigned int programIndex);
};

#endif
