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


#include "Object.h"

#include <glm/gtc/matrix_transform.hpp>


namespace gland{

  Object::Object():
    translation_(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f))),
    rotation_(glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1.0f))),
    scale_(glm::scale(glm::mat4(1.0f), glm::vec3(1.0f))) {}


  glm::mat4 Object::translate(const glm::vec3 &translation){
    translation_ = glm::translate(translation_, translation);
    return translation_;
  }

  glm::mat4 Object::translate(const float x, const float y, const float z){
    return this->translate(glm::vec3(x, y, z));
  }

  glm::mat4 Object::rotate(const float angle, const glm::vec3 &axis){
    rotation_ = glm::rotate(rotation_, angle, axis);
    return rotation_;
  }


  glm::mat4 Object::rotate(const float angle, const float x, const float y, const float z){
    return this->rotate(angle, glm::vec3(x, y, z));
  }

  glm::mat4 Object::scale(const glm::vec3 &scale){
    scale_ = glm::scale(scale_, scale);    
    return scale_;
  }

  glm::mat4 Object::scale(const float x, const float y, const float z){
    return this->scale(glm::vec3(x, y, z));
  }


  void Object::setPosition(const float x, const float y, const float z){
    translation_[3][0] = x;
    translation_[3][1] = y;
    translation_[3][2] = z;
  }
    
  void Object::setPosition(glm::vec3 &position){
    this->setPosition(position.x, position.y, position.z);
  }

  glm::vec3 Object::getPosition(){
    return glm::vec3(translation_[3][0], translation_[3][1], translation_[3][2]);    
  }


  glm::mat4 Object::getScaleMatrix(){
    return scale_;
  }

  glm::mat4 Object::getModelMatrix(){
    return translation_ * rotation_;
  }

  glm::mat4 Object::getTranslationMatrix(){
    return translation_;
  }

  glm::mat4 Object::getRotationMatrix(){
    return rotation_;
  }

  void Object::setRotationMatrix(glm::mat4 &m){
    rotation_ = m;
  }
  void Object::setTranslationMatrix(glm::mat4 &m){
    translation_ = m;
  }
}
