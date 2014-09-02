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

#include "SMController.h"

namespace gland{
  SMController::SMController(){}     

  SMController::SMController(std::map<std::string, gland::Param> &params, GLFWwindow* window):
    params_(&params),
    window_(window){}
        
  void SMController::keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_M && action == GLFW_PRESS){
      (*params_)["manualLight"] = !((*params_)["manualLight"]);
    } 
    if (key == GLFW_KEY_J && action == GLFW_PRESS){
      (*params_)["renderDepth"] = !((*params_)["renderDepth"]);
    }
  }

  void SMController::handleInput(){
    if (glfwGetKey (window_, GLFW_KEY_O)) {
      (*params_)["offsetFactor"] += 0.1f;
    }
    if (glfwGetKey (window_, GLFW_KEY_L)) {
      (*params_)["offsetFactor"] -= 0.1f;
    }
    if (glfwGetKey (window_, GLFW_KEY_I)) {
      (*params_)["offsetUnits"] += 0.1f;
    }
    if (glfwGetKey (window_, GLFW_KEY_K)) {
      (*params_)["offsetUnits"] -= 0.1f;
    }
    if (glfwGetKey (window_, GLFW_KEY_B)) {
      (*params_)["angle"] += 10.0f;
    }
    if (glfwGetKey (window_, GLFW_KEY_V)) {
      (*params_)["angle"] -= 10.0f;
    }
    if (glfwGetKey (window_, GLFW_KEY_T)) {
      (*params_)["lightRadius"] += 0.2f;
    }
    if (glfwGetKey (window_, GLFW_KEY_G)) {
      (*params_)["lightRadius"] -= 0.2f;
    }

    /*      if (mReady && glfwGetKey (window_, GLFW_KEY_M)) {
            params_->manualLight = !params_->manualLight;
            mReady = false;
            } else {
            mReady = true;
            }*/
  }

  void SMController::generateHelpText(std::vector<std::string> &help){
    help.push_back("M: Toggle light control");
    help.push_back("O/L: Polygon offset factor");
    help.push_back("I/K: Polygon offset units");
    help.push_back("V/B: Move light (manual)");
  }
}
