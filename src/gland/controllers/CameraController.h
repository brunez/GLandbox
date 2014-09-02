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

#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H 

#include <memory>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Controller.h"


namespace gland{

  class CameraController : public Controller <CameraController>{

  public:
    CameraController(){}
    CameraController(std::shared_ptr<Camera> &camera, GLFWwindow* window);
    void handleInput();
    virtual void keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods){}
    void generateHelpText(std::vector<std::string> &help);
  private:
    std::shared_ptr<Camera> camera_;
    GLFWwindow *window_;
    double mouseX_ = 0.0;
    double mouseY_ = 0.0;
    double mousePreX_ = 0.0;
    double mousePreY_ = 0.0;
    double mouseDX_ = 0.0;
    double mouseDY_ = 0.0;
    float cameraSpeed_ = 1e-1;
    bool first_;
  };
}

#endif
