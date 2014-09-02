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

#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "Material.h"


namespace gland{

  class Object{

  public:

    explicit Object();

    /**
     * Modifies the translation matrix with the given vector
     * @param translation The translation vector
     */
    glm::mat4 translate(const glm::vec3 &translation);

    /**
     * Modifies the translation matrix with the given vector
     * @param x,y,z The components of the translation vector
     */
    glm::mat4 translate(const float x, const float y, const float z);

    /**
     * Modifies the rotation matrix with the given vector
     * @param angle The angle of the rotation in radians
     * @param axis The vector around which the rotation takes place
     */
    glm::mat4 rotate(const float angle, const glm::vec3 &axis);

    /**
     * Modifies the rotation matrix with the given vector
     * @param angle The angle of the rotation in radians
     * @param x,y,z The vector around which the rotation takes place
     */
    glm::mat4 rotate(const float angle, const float x, const float y, const float z);

    /**
     * Modifies the scale matrix with the given vector
     * @param scale The scale vector
     */    
    glm::mat4 scale(const glm::vec3 &scale);

    /**
     * Modifies the scale matrix with the given vector
     * @param x,y,z The scale vector
     */    
    glm::mat4 scale(const float x, const float y, const float z);

    glm::vec3 getPosition();

    void setPosition(const float x, const float y, const float z);
    void setPosition(glm::vec3 &position);


    glm::mat4 getScaleMatrix();
    glm::mat4 getModelMatrix();
    glm::mat4 getTranslationMatrix();
    glm::mat4 getRotationMatrix();
    void setRotationMatrix(glm::mat4 &m);
    void setTranslationMatrix(glm::mat4 &m);

  private:
    //TODO Merge translation and rotation?
    glm::mat4 translation_;
    glm::mat4 rotation_;
    glm::mat4 scale_;

    glm::vec3 ambient_;
    glm::vec3 diffuse_;
    glm::vec3 specular_;

  };
}
#endif
