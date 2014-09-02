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

#ifndef SMCONTROLLER_H
#define SMCONTROLLER_H

#include <vector>
#include <map>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include "Controller.h"
#include "Param.h"

namespace gland{

  class SMController : public Controller<SMController>{
  public:
    SMController();
    
    SMController(std::map<std::string, gland::Param> &params, GLFWwindow* window);  
    void keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods);  
    void handleInput();
    void generateHelpText(std::vector<std::string> &help);
    
  private:
    std::map<std::string, gland::Param> * params_;
    GLFWwindow *window_;
    bool mReady = true;
  }; 
}

#endif
