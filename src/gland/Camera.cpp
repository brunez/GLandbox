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
 * File Camera.cpp
 * Author: bruno
 */

#include <iostream>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h" 
#include "glm_print.h"

namespace gland{

  Camera::Camera():
    position_(glm::vec3(0.0f, 0.0f, 0.1f)),
    up_(glm::vec3(0.0f, 1.0f, 0.0f)){

    //TODO Temporary. Gotta get the window aspect ration
    float ratioW = 16;
    float ratioH = 9;

    near_ = 0.1f; // clipping plane
    far_ = 1000.0f; // clipping plane
    fov_ = 45.0 * ONE_DEG_IN_RAD; // convert 67 degrees to radians
    aspectRatio_ = (float)ratioW / (float)ratioH; // aspect ratio

    //TODO  rotation and forward must be coherent
    glm::vec3 eulerAngles(0.0, 0.0, 0.0);
    quaternion_ = glm::quat(eulerAngles);

    this->updateProjectionMatrix();

    //GLM method for projection matrix generation
    //glm::mat4 projectionMatrix_2 = glm::perspective<float>(fov_, aspectRatio_, near_, far_);

    this->update();

  }


  glm::mat4 Camera::lookAt(const glm::vec3 &eye, const glm::vec3 &target, const glm::vec3 &up){
    position_ = eye;
    //Distance from the camera to the target point
    glm::vec3 distance = target - position_;
    //The forward vector is the normalized distance
    glm::vec3 f = glm::normalize(distance);
    forward_ = f;
    //Up and right vectors
    glm::vec3 r = glm::cross(forward_, up_);    
    //We re-calculate the up vector for consistency: 
    up_ = glm::normalize(glm::cross(r, f));    

    this->viewMatrix_ = glm::lookAt(eye, target, up);

    //Define rotation and translation matrices
    //Column major, indexing by columns
    rotationMatrix_ = {
      r.x, up_.x, -f.x, 0.0f,
      r.y, up_.y, -f.y, 0.0f,
      r.z, up_.z, -f.z, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
    };
    translationMatrix_ = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      -position_.x, -position_.y, -position_.z, 1.0f
    };

    //Compute the view matrix
    this->update();
    //this->updateCameraMatrix();
  }

  void Camera::lookAt(const float x, const float y, const float z){
    glm::vec3 target(x, y, z);
    this->lookAt(position_, glm::vec3(x, y, z), up_);
  }

  void Camera::lookAt(glm::vec3 &target){
    this->lookAt(position_, target, up_);
  }

  void Camera::lookAt(glm::vec3 &target, glm::vec3 &up){
    up_ = up;
    this->lookAt(position_, target, up_);
  }


  void Camera::update(){
    viewMatrix_ = rotationMatrix_*translationMatrix_;    
    //      quaternion_ = glm::toQuat(rotationMatrix_);
  }

  void Camera::moveTo(glm::vec3 &position){
    position_ = position;
  }

  void Camera::moveTo(const float x, const float y, const float z){
    position_ = glm::vec3(x, y, z);
  }

  void Camera::updateProjectionMatrix(){
    // matrix components
    float range = glm::tan(fov_ * 0.5f) * near_;
    float sx = (2.0f * near_) / (range * aspectRatio_ + range * aspectRatio_);
    float sy = near_ / range;
    float sz = -(far_ + near_) / (far_ - near_);
    float pz = -(2.0f * far_ * near_) / (far_ - near_);
    float projVals[] = {
      sx, 0.0f, 0.0f, 0.0f,
      0.0f, sy, 0.0f, 0.0f,
      0.0f, 0.0f, sz, -1.0f,
      0.0f, 0.0f, pz, 0.0f
    };
    projectionMatrix_ = glm::make_mat4(projVals);    

    //this->updateCameraMatrix();
  }

  void Camera::rotate(const float x, const float y, const float z){
    glm::vec3 r = glm::cross(forward_, up_);
    quaternion_ = glm::rotate(quaternion_, y, r);
    up_ = glm::cross(r, forward_);        
    quaternion_ = glm::rotate(quaternion_, x, (glm::vec3(0.0f, 1.0f, 0.0f)));
    forward_ = glm::cross(r, up_);
    rotationMatrix_ = glm::toMat4(quaternion_);
        
    const float *rotVals = (const float*)glm::value_ptr(rotationMatrix_);
    forward_.x = -rotVals[2];
    forward_.y = -rotVals[6];
    forward_.z = -rotVals[10];
    up_.x = rotVals[1];
    up_.y = rotVals[5];
    up_.z = rotVals[9];
    //        up_ = glm::cross(r, forward_);            
  }

  void Camera::right(const float distance){

    //TODO Use right vector computed in view changes?

    glm::vec3 r = glm::cross(forward_, up_); 
    position_ += r*distance;
    translationMatrix_[3][0] = -position_.x;
    translationMatrix_[3][1] = -position_.y;
    translationMatrix_[3][2] = -position_.z;

  }

  void Camera::forward(const float distance){
        
    position_ += forward_*distance;
    translationMatrix_[3][0] = -position_.x;
    translationMatrix_[3][1] = -position_.y;
    translationMatrix_[3][2] = -position_.z;
  }

  void Camera::up(const float distance){

    position_ += up_*distance;
    translationMatrix_[3][0] = -position_.x;
    translationMatrix_[3][1] = -position_.y;
    translationMatrix_[3][2] = -position_.z;
  }

  void Camera::down(const float distance){
    position_ += up_*-distance;
    translationMatrix_[3][0] = -position_.x;
    translationMatrix_[3][1] = -position_.y;
    translationMatrix_[3][2] = -position_.z;
  }

  void Camera::floatUp(const float distance){
    position_ += glm::vec3(0.0, 1.0, 0.0)*distance;
    translationMatrix_[3][0] = -position_.x;
    translationMatrix_[3][1] = -position_.y;
    translationMatrix_[3][2] = -position_.z;
  }

  void Camera::sinkDown(const float distance){
    position_ += glm::vec3(0.0, 1.0, 0.0)*-distance;
    translationMatrix_[3][0] = -position_.x;
    translationMatrix_[3][1] = -position_.y;
    translationMatrix_[3][2] = -position_.z;
  }


  void Camera::setQuaternion(const double x, const double y, const double z){
    glm::vec3 eulerAngles(x, y, z);
    quaternion_ = glm::quat(eulerAngles);
  }

  //**************************************************
  // Getters and setters
  //**************************************************

  glm::mat4 Camera::getViewMatrix(){
    return viewMatrix_;
  }

  glm::mat4 Camera::getProjectionMatrix(){
    return projectionMatrix_;
  }

  glm::mat4 Camera::getCameraMatrix(){
    return cameraMatrix_;
  }


  void Camera::setNear(const float near){
    near_ = near;
  }

  void Camera::setFar(const float far){
    far_ = far;
  }

  void Camera::setFOV(const float fov){
    fov_ = fov;
  }

  void Camera::setAspectRatio(const float aspectRatio){
    aspectRatio_ = aspectRatio;
  }

  void Camera::setAspectRatio(const float proportionalWidth, const float proportionalHeigth){
    aspectRatio_ = proportionalWidth / proportionalHeigth;
  }

  void Camera::printVectors(){
    glm::vec3 r = glm::cross(forward_, up_);
    std::cout << "Right: " << r.x << ", " << r.y << ", " << r.z << std::endl;
    std::cout << "Up: " << up_.x << ", " << up_.y << ", " << up_.z << std::endl;
    std::cout << "Forward: " << forward_.x << ", " << forward_.y << ", " << forward_.z << std::endl;
  }

}
