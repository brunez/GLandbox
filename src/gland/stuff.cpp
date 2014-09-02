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

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

#include "stuff.h"

namespace gland{

    GLuint makeBuffer(const std::string &filePath){
        GLuint bufferName;
        glGenBuffers(1, &bufferName);
        makeBuffer(bufferName, filePath);
        return bufferName;
    }

    void makeBuffer(GLuint bufferName, const std::string &filePath){
        std::vector<float> values = loadRaw(filePath);
        float data[values.size()];
        std::copy(values.begin(), values.end(), data);
        glBindBuffer(GL_ARRAY_BUFFER, bufferName);
        glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    }

    std::vector<float> loadRaw(const std::string &filePath){
        std::vector<float> floats;
        std::ifstream infile(filePath);
        std::string s;
        while (infile && getline(infile, s)){
            std::stringstream ss(s);
            while (ss && getline(ss, s, ',')){
                if (!std::regex_match(s, std::regex(" +"))) {
                    floats.push_back(std::stof(s));
                }
                
            }
        }
        if (!infile.eof()){
            std::cerr << "Error reading file\n";
        }
        return floats;
    }


    void spotError(bool verbose){
        GLuint e;
        if ((e = glGetError()) != GL_NO_ERROR) {
            std::cerr << "****Error spotted****: " << e << std::endl;
            std::cerr << gluErrorString(e) << std::endl;
        } else if(verbose){
          std::cerr << "====No errors====: " << e << std::endl;
        }
    }
}
