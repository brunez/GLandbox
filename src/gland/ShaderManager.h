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
 * File ShaderManager.h
 * Author: bruno
 */

#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <fstream>
#include <vector>

#include <GL/glew.h>

#include "ShaderDough.h"

namespace gland{
    class ShaderManager {
    public:
        ShaderManager();
    
        /**
         * Load and compile shader code from a file
         */
        GLuint loadShader(const std::string &shaderCode, GLenum shaderType);

        /**
         * Create a shader program from a list of shaders
         */
        GLuint createProgram(std::vector<GLuint> &shaders);

        /**
         * Create a shader program from a ShaderDough object. If the program is separable, the pipeline must be manually created
         */
        //TODO Make sure separability is correctly supported
        GLuint cookDough(gland::ShaderDough &dough, bool separable=false);

    private:
    };
}
#endif //SHADERMANAGER_H
