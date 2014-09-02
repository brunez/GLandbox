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

#include <GL/glew.h>

//TODO Use cpp libraries
#include <stdio.h> 
#include <fstream>
#include <iostream>
#include "gllog.h"

namespace gllog{
#define GL_LOG_FILE "gl.log"

    bool restart_gl_log () {
        time_t now = time (NULL);
        char* date = ctime (&now);
        std::ofstream out;
        out.open(GL_LOG_FILE);
        out <<  GL_LOG_FILE << " log. Local time " << date << std::endl;
        out.close(); 
        return true;
    }

    bool gl_log (const char* message, const char* filename, int line) {
        std::ofstream out;
        out.open(GL_LOG_FILE, std::fstream::out | std::fstream::app);
        out << filename << ":" << line << message << std::endl;
        out.close();
        return true;  
    }

    void glfw_error_callback (int error, const char* description) {
        fputs (description, stderr);
        gl_log (description, __FILE__, __LINE__);  
    }

    void log_gl_params () {
        GLenum params[] = {
            GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,
            GL_MAX_CUBE_MAP_TEXTURE_SIZE,
            GL_MAX_DRAW_BUFFERS,
            GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,
            GL_MAX_TEXTURE_IMAGE_UNITS,
            GL_MAX_TEXTURE_SIZE,
            GL_MAX_VARYING_FLOATS,
            GL_MAX_VERTEX_ATTRIBS,
            GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,
            GL_MAX_VERTEX_UNIFORM_COMPONENTS,
            GL_MAX_VIEWPORT_DIMS,
            GL_STEREO,
        };
        const char* names[] = {
            "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",
            "GL_MAX_CUBE_MAP_TEXTURE_SIZE",
            "GL_MAX_DRAW_BUFFERS",
            "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",
            "GL_MAX_TEXTURE_IMAGE_UNITS",
            "GL_MAX_TEXTURE_SIZE",
            "GL_MAX_VARYING_FLOATS",
            "GL_MAX_VERTEX_ATTRIBS",
            "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",
            "GL_MAX_VERTEX_UNIFORM_COMPONENTS",
            "GL_MAX_VIEWPORT_DIMS",
            "GL_STEREO",
        };
        gl_log ("GL Context Params:", __FILE__, __LINE__);
        char msg[256];
        // integers - only works if the order is 0-10 integer return types
        for (int i = 0; i < 10; i++) {
            int v = 0;
            glGetIntegerv (params[i], &v);
            sprintf (msg, "%s %i", names[i], v);
            gl_log (msg, __FILE__, __LINE__);
        }
        // others
        int v[2];
        v[0] = v[1] = 0;
        glGetIntegerv (params[10], v);
        sprintf (msg, "%s %i %i\n", names[10], v[0], v[1]);
        gl_log (msg, __FILE__, __LINE__);
        unsigned char s = 0;
        glGetBooleanv (params[11], &s);
        sprintf (msg, "%s %i\n", names[11], (unsigned int)s);
        gl_log (msg, __FILE__, __LINE__);
        gl_log ("-----------------------------\n", __FILE__, __LINE__);
    }

    //TODO Function that prints all states https://www.opengl.org/sdk/docs/man/docbook4/xhtml/glGet.xml

    //TODO Figure out where to put these functions. If they're not GLFW callbacks, they can be in a class
    void update_fps_counter (GLFWwindow* window) {
        static double previous_seconds = glfwGetTime ();
        static int frame_count;
        double current_seconds = glfwGetTime ();
        double elapsed_seconds = current_seconds - previous_seconds;
        if (elapsed_seconds > 0.25) {
            previous_seconds = current_seconds;
            double fps = (double)frame_count / elapsed_seconds;
            char tmp[128];
            sprintf (tmp, "opengl @ fps: %.2f", fps);
            glfwSetWindowTitle (window, tmp);
            frame_count = 0;
        }
        frame_count++;
    }

    void _print_shader_info_log (unsigned int shaderIndex) {
        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetShaderInfoLog (shaderIndex, max_length, &actual_length, log);
        printf ("shader info log for GL index %i:\n%s\n", shaderIndex, log);
    }

    void _print_programme_info_log (unsigned int programIndex) {
        int max_length = 2048;
        int actual_length = 0;
        char log[2048];
        glGetProgramInfoLog (programIndex, max_length, &actual_length, log);
        printf ("program info log for GL index %i:\n%s", programIndex, log);
    }

    /*
      void print_all () {
      printf ("--------------------\nshader programme %i info:\n", m_programme_idx);
      int params = -1;
      glGetProgramiv (m_programme_idx, GL_LINK_STATUS, &params);
      char value[32];
      if (GL_TRUE == params) {
      strcpy (value, "GL_TRUE");
      } else {
      strcpy (value, "GL_FALSE");
      }
      printf ("GL_LINK_STATUS = %s\n", value);
  
      glGetProgramiv (m_programme_idx, GL_ATTACHED_SHADERS, &params);
      printf ("GL_ATTACHED_SHADERS = %i\n", params);
      if (m_has_vertex_shader) {
      printf ("  vertex shader index %i. file name: %s\n", m_vertex_shader_idx, m_vertex_shader_file_name);
      }
      if (m_has_fragment_shader) {
      printf ("  fragment shader index %i. file name: %s\n", m_fragment_shader_idx, m_fragment_shader_file_name);
      }
  
      glGetProgramiv (m_programme_idx, GL_ACTIVE_ATTRIBUTES, &params);
      printf ("GL_ACTIVE_ATTRIBUTES = %i\n", params);
      for (unsigned int i = 0; i < params; i++) {
      char name[64];
      int max_length = 64;
      int actual_length = 0;
      int size = 0;
      GLenum type;
      glGetActiveAttrib (m_programme_idx, i, max_length, &actual_length, &size, &type, name);
      if (size > 1) {
      for (int j = 0; j < size; j++) {
      char long_name[64];
      sprintf (long_name, "%s[%i]", name, j);
      int location = glGetAttribLocation (m_programme_idx, long_name);
      printf ("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string (type), long_name, location);
      }
      } else {
      int location = glGetAttribLocation (m_programme_idx, name);
      printf ("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string (type), name, location);
      }
      }
  
      glGetProgramiv (m_programme_idx, GL_ACTIVE_UNIFORMS, &params);
      printf ("GL_ACTIVE_UNIFORMS = %i\n", params);
      for (unsigned int i = 0; i < params; i++) {
      char name[64];
      int max_length = 64;
      int actual_length = 0;
      int size = 0;
      GLenum type;
      glGetActiveUniform (m_programme_idx, i, max_length, &actual_length, &size, &type, name);
      if (size > 1) {
      for (int j = 0; j < size; j++) {
      char long_name[64];
      sprintf (long_name, "%s[%i]", name, j);
      int location = glGetUniformLocation (m_programme_idx, long_name);
      printf ("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string (type), long_name, location);
      }
      } else {
      int location = glGetUniformLocation (m_programme_idx, name);
      printf ("  %i) type:%s name:%s location:%i\n", i, GL_type_to_string (type), name, location);
      }
      }
  
      _print_programme_info_log ();
      }
    */

    const char* GL_type_to_string (unsigned int type) {
        if (GL_FLOAT == type) {
            return "GL_FLOAT";
        }
        if (GL_FLOAT_VEC2 == type) {
            return "GL_FLOAT_VEC2";
        }
        if (GL_FLOAT_VEC3 == type) {
            return "GL_FLOAT_VEC3";
        }
        if (GL_FLOAT_VEC4 == type) {
            return "GL_FLOAT_VEC4";
        }
        if (GL_FLOAT_MAT2 == type) {
            return "GL_FLOAT_MAT2";
        }
        if (GL_FLOAT_MAT3 == type) {
            return "GL_FLOAT_MAT3";
        }
        if ( GL_FLOAT_MAT4 == type) {
            return "GL_FLOAT_MAT4";
        }
        if (GL_INT == type) {
            return "GL_INT";
        }
        if (GL_BOOL == type) {
            return "GL_BOOL";
        }
        if (GL_SAMPLER_2D == type) {
            return "GL_SAMPLER_2D";
        }
        if (GL_SAMPLER_3D == type) {
            return "GL_SAMPLER_3D";
        }
        if (GL_SAMPLER_CUBE == type) {
            return "GL_SAMPLER_CUBE";
        }
        if (GL_SAMPLER_2D_SHADOW == type) {
            return "GL_SAMPLER_2D_SHADOW";
        }
        return "OTHER";
    }

    bool is_valid (unsigned int programIndex) {
        glValidateProgram (programIndex);
        int params = -1;
        glGetProgramiv (programIndex, GL_VALIDATE_STATUS, &params);
        if (GL_TRUE != params) {
            printf ("program %i GL_VALIDATE_STATUS = GL_FALSE\n", programIndex);
            _print_programme_info_log (programIndex);
            return false;
        }
        printf ("program %i GL_VALIDATE_STATUS = GL_TRUE\n", programIndex);
        return true;
    }
}
