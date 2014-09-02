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

#include "Material.h"

namespace gland{

  void Material::setAmbient(glm::vec3 ambient){
    ambient_ = ambient;
  }

  void Material::setAmbient(float r, float g, float b){
    this->setAmbient(glm::vec3(r, g, b));
  }

  void Material::setDiffuse(glm::vec3 diffuse){
    diffuse_ = diffuse;
  }

  void Material::setDiffuse(float r, float g, float b){
    this->setDiffuse(glm::vec3(r, g, b));
  }

  void Material::setSpecular(glm::vec3 specular){
    specular_ = specular;
  }

  void Material::setSpecular(float r, float g, float b){
    this->setSpecular(glm::vec3(r, g, b));
  }

  glm::vec3 Material::getAmbient(){
    return ambient_;
  }

  glm::vec3 Material::getDiffuse(){
    return diffuse_;
  }

  glm::vec3 Material::getSpecular(){
    return specular_;
  }


}
