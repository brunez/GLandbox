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

#include <string>
#include <vector>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include <GL/glew.h>

namespace gland{

    /**
     * Creates a buffer object and allocates its data from the
     * given file
     */
    GLuint makeBuffer(const std::string &filePath);

    /**
     * Binds the given buffer object and allocates its data from the given
     * file 
     */
    void makeBuffer(GLuint bufferName, const std::string &filePath);

    /**
     * Loads float data from a comma separated value file. It supports
     * newline characters.
     */
    std::vector<float> loadRaw(const std::string &filePath);

    /**
     * When this function is called it informs of whether glGetError()
     * returned an error, and if so it prints the corresponding message
     */
    void spotError(bool verbose = false);

    // trim from start
    static inline std::string &ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
        return s;
    }

    // trim from end
    static inline std::string &rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
        return s;
    }

    // trim from both ends
    static inline std::string &trim(std::string &s) {
        return ltrim(rtrim(s));
    }
}
