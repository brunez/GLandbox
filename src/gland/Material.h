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

#ifndef MATERIAL_H
#define MATERIAL_H

#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Object.h"

namespace gland{

  class Material{

  public:

    explicit Material();

    void setAmbient(glm::vec3 ambient);
    void setAmbient(float r, float g, float b);
    void setDiffuse(glm::vec3 diffuse);
    void setDiffuse(float r, float g, float b);
    void setSpecular(glm::vec3 specular);
    void setSpecular(float r, float g, float b);
    glm::vec3 getAmbient();
    glm::vec3 getDiffuse();
    glm::vec3 getSpecular();

  private:
    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;
  };
}
#endif
