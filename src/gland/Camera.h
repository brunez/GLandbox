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
 * File Camera.h
 * Author: bruno
 */

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

namespace gland{

  
#define ONE_DEG_IN_RAD (2.0 * M_PI) / 360.0 // 0.017444444
  
  class Camera {
  public:
    Camera();

    /**
     * Makes the camera point to the specified location
     * @param target The point to look at
     */
    glm::mat4 lookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up);

    /**
     * Makes the camera point to the specified location
     * @param x The x coordinate of the target
     * @param y The y coordinate of the target
     * @param z The z coordinate of the target
     */
    void lookAt(const float x, const float y, const float z);

    /**
     * Makes the camera point to the specified location
     * @param target A three-dimensional vector representing the target
     */
    void lookAt(glm::vec3 &target);


    /**
     * Makes the camera point to the specified location
     * @param target The point to look at
     * @param up The up vector of the camera
     */
    void lookAt(glm::vec3 &target, glm::vec3 &up);

    /**
     * Moves the camera to the specified location
     * @param position The point to move to
     */
    void moveTo( glm::vec3 &position);

    /**
     * Moves the camera to the specified location
     * @param x The x coordinate of the target location
     * @param y The y coordinate of the target location
     * @param z The z coordinate of the target location
     */
    void moveTo(const float x, const float y, const float z);

    void updateProjectionMatrix(const float near, const float far);

    void updateProjectionMatrix();

    /**
     * Returns the view matrix
     */
    glm::mat4 getViewMatrix();

    /**
     * Returns the projection matrix
     */
    glm::mat4 getProjectionMatrix();

    /**
     * Returns the camera matrix (projection*view)
     */
    glm::mat4 getCameraMatrix();

    void update();

    //TODO
    void pitch(const float degrees);

    //TODO
    void yaw(const float degrees);

    //TODO
    void roll(const float degrees);

    void right(const float distance);

    void left(const float distance);

    void forward(const float distance);

    void backward(const float distance);

    void up(const float distance);

    void down(const float distance);

    void floatUp(const float distance);

    void sinkDown(const float distance);

    void rotate(const float x, const float y, const float z);        
        
    void setNear(const float near);

    void setFar(const float far);

    void setFOV(const float fov);

    void setQuaternion(const double x, const double y, const double z);

    void printVectors();

    //TODO Do I want to change this? Maybe it should only be
    //called when the window is resized
    void setAspectRatio(const float aspectRatio);

    void setAspectRatio(const float proportionalWidth, const float proportianlHeigth);

  private:

    /**
     * The position of the camera
     */
    glm::vec3 position_;

    /**
     * The up vector of the camera
     */
    glm::vec3 up_;

    /**
     * The forward vector of the camera
     */
    glm::vec3 forward_;
        
    /**
     * Quaternion representing rotation
     */
    glm::quat quaternion_;

    //TODO Model matrix vs Tr + Rot
    //TODO Find out the fastest way to manage these matrices
    //Maybe it's too expensive to extract the float array
    /**
     * The view matrix that corresponds to this camera
     */
    glm::mat4 viewMatrix_;

    /**
     * The projection matrix that corresponds to this camera
     */
    glm::mat4 projectionMatrix_;

    /**
     * The camera matrix (projection*view);
     */
    glm::mat4 cameraMatrix_;


    /**
     * The translation matrix
     */
    glm::mat4 translationMatrix_;


    /**
     * The rotation matrix
     */
    glm::mat4 rotationMatrix_;


    /**
     * The near value of the projection matrix
     */
    float near_;

    /**
     * The far value of the projection matrix
     */
    float far_;

    /**
     * The fov value of the projection matrix
     */
    float fov_;


    /**
     * The aspect ratio
     */
    float aspectRatio_;


  };

}
#endif //CAMERA_H
