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

/*
 * File ShaderManager.cpp
 * Author: bruno
 */

#include <iostream>

#include "ShaderManager.h" 
#include "gllog.h"

namespace gland{

    ShaderManager::ShaderManager(){}
    GLuint ShaderManager::loadShader(const std::string &shaderCode, GLenum shaderType){
        //TODO Is it necessary to pass shader length to glShaderSource ??? Works if not
        //Garbage appeared when I made the conversion to char* inside the load file method...
        //const GLint lenvshader = shaderCode.length();
        const char* shaderCodeChars = shaderCode.c_str();

        //Compile the vertex shader
        unsigned int shaderId = glCreateShader (shaderType);
        glShaderSource (shaderId, 1, &shaderCodeChars, nullptr);
        glCompileShader (shaderId);

        // check for compile errors
        int params = -1;
        glGetShaderiv (shaderId, GL_COMPILE_STATUS, &params);
        if (GL_TRUE != params) {
            fprintf (stderr, "ERROR: GL shader index %i did not compile\n", shaderId);
            gllog:: _print_shader_info_log (shaderId);
            return -1; // or exit or something
        }

        return shaderId;
    }

    GLuint ShaderManager::createProgram(std::vector<GLuint> &shaders){
        //TODO
    }

  GLuint ShaderManager::cookDough(gland::ShaderDough &dough, bool separable){
        GLuint shaderProgram = glCreateProgram();
        std::vector<GLuint> shaders = dough.getShaders();
        for(GLuint i : shaders){
            glAttachShader (shaderProgram, i);
        }
        if(separable){
          glProgramParameteri(shaderProgram, GL_PROGRAM_SEPARABLE, GL_TRUE);
        }
        glLinkProgram (shaderProgram);

        // check if link was successful
        int params = -1;
        glGetProgramiv (shaderProgram, GL_LINK_STATUS, &params);
        if (GL_TRUE != params) {
            fprintf (
                     stderr,
                     "ERROR: could not link shader programme GL index %i\n",
                     shaderProgram
                     );
            gllog::_print_programme_info_log (shaderProgram);
        }
        //Clean up

        for(GLuint i : shaders){
            glDeleteShader(i);
        }
        return shaderProgram;
    }
}
