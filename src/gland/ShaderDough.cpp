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
 * File ShaderDough.cpp
 * Author: bruno
 */

#include <fstream>

#include "ShaderDough.h" 
#include "gllog.h"

namespace gland{
    ShaderDough::ShaderDough(){}

    void ShaderDough::addShader(const std::string &shaderCode, GLenum shaderType){
        const char* shaderCodeChars = shaderCode.c_str();
        GLuint shaderObject = glCreateShader (shaderType);
        glShaderSource (shaderObject, 1, &shaderCodeChars, nullptr);
        glCompileShader (shaderObject);

        // check for compile errors
        int params = -1;
        glGetShaderiv (shaderObject, GL_COMPILE_STATUS, &params);
        if (GL_TRUE != params) {
            fprintf (stderr, "ERROR: GL shader index %i did not compile\n", shaderObject);
            gllog:: _print_shader_info_log (shaderObject);
        }

        //Add the shader to this object's list
        shaders_.push_back(shaderObject);
    }

    void ShaderDough::addShaderFromFile(const std::string &filePath, GLenum shaderType){
        std::ifstream in(filePath.c_str());
        std::string contents((std::istreambuf_iterator<char> (in)), 
                             std::istreambuf_iterator<char>());
        in.close();    
        this->addShader(contents, shaderType);
    }

    std::vector<GLuint> ShaderDough::getShaders(){
        return shaders_;
    }
}
