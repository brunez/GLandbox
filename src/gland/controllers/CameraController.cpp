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

#include "CameraController.h"


namespace gland{

  CameraController::CameraController(std::shared_ptr<gland::Camera> &camera, GLFWwindow* window):
    camera_(camera),
    window_(window),
    first_(true){}

  void CameraController::handleInput(){

    if (glfwGetKey (window_, GLFW_KEY_A)) {
      camera_->right(-cameraSpeed_);
    }
    if (glfwGetKey (window_, GLFW_KEY_D)) {
      camera_->right(cameraSpeed_);
    }
    if (glfwGetKey (window_, GLFW_KEY_W)) {
      camera_->forward(cameraSpeed_);
    }
    if (glfwGetKey (window_, GLFW_KEY_S)) {
      camera_->forward(-cameraSpeed_);
    }
    if (glfwGetKey (window_, GLFW_KEY_E)) {
      camera_->up(cameraSpeed_);
    }
    if (glfwGetKey (window_, GLFW_KEY_C)) {
      camera_->up(-cameraSpeed_);
    }
    if (glfwGetKey (window_, GLFW_KEY_Q)) {
      camera_->floatUp(cameraSpeed_);
    }
    if (glfwGetKey (window_, GLFW_KEY_Z)) {
      camera_->floatUp(-cameraSpeed_);
    }

    if (glfwGetKey (window_, GLFW_KEY_H)) {
      camera_->moveTo(0.0f, 0.0f, 1.0f);
      camera_->lookAt(0.0f, 0.0f, 0.0f);
    }

    if (glfwGetKey (window_, GLFW_KEY_R)) {
      cameraSpeed_ *= 1.01F;
    }

    if (glfwGetKey (window_, GLFW_KEY_F)) {
      cameraSpeed_ *= 0.99F;
    }

    if(!first_){
      glfwGetCursorPos(window_, &mouseX_, &mouseY_);
      mouseDX_ = mouseX_ - mousePreX_;
      mouseDY_ = mouseY_ - mousePreY_;
      mousePreX_ = mouseX_;
      mousePreY_ = mouseY_;
      camera_->rotate(mouseDX_/(18.0), mouseDY_/(18.0), 0.0);
    } else {
      first_ = false;
    }
  }

  void CameraController::generateHelpText(std::vector<std::string> &help){
    help.push_back("WASD-QZEC: Move");
    help.push_back("R/F: Change speed");
    help.push_back("H: Go to origin");
  }

}
