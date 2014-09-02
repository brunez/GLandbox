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


#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "glm_print.h"

std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "{" 
        << vec.x << " " << vec.y << " "<< vec.z 
        << "}";

    return out;
}


std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
    out << "{" 
        << vec.x << " " << vec.y << " "<< vec.z << " " << vec.w
        << "}";

    return out;
}


std::ostream &operator<< (std::ostream &out, const glm::mat3 &mat) {
  out << mat[0][0] << "\t" << mat[0][1] << "\t" << mat[0][2] << std::endl
  << mat[1][0] << "\t" << mat[1][1] << "\t" << mat[1][2] << std::endl
  << mat[2][0] << "\t" << mat[2][1] << "\t" << mat[2][2] << std::endl;
    return out;
}

std::ostream &operator<< (std::ostream &out, const glm::mat4 &mat) {
  out << mat[0][0] << "\t" << mat[0][1] << "\t" << mat[0][2] << "\t" << mat[0][3] << "\t" << std::endl
      << mat[1][0] << "\t" << mat[1][1] << "\t" << mat[1][2] << "\t" << mat[1][3] << "\t" << std::endl
      << mat[2][0] << "\t" << mat[2][1] << "\t" << mat[2][2] << "\t" << mat[2][3] << "\t" << std::endl
      << mat[3][0] << "\t" << mat[3][1] << "\t" << mat[3][2] << "\t" << mat[3][3] << "\t" << std::endl;
    return out;
}

