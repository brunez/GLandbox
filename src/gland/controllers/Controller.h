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

#ifndef CONTROLLER_H
#define CONTROLLER_H 
#include <iostream>
#include <memory>
#include <vector>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

namespace gland{

  class BaseController{
  public:
    virtual void handleInput() = 0;
    virtual void keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;

    virtual void generateHelpText(std::vector<std::string> &help){
      std::cerr << "This controller has no associated help text" << std::endl;
    }

  };

template <class T>  
    class Controller : public BaseController{
  public:
  
  virtual void handleInput(){}
  virtual void keyCallbackImpl(GLFWwindow* window, int key, int scancode, int action, int mods){}

  void subCallbacks(GLFWwindow* window, int key, int scancode, int action, int mods){
    for(std::shared_ptr<BaseController> c : subControllers_){
      c->keyCallbackImpl(window, key, scancode, action, mods);
    }
  };

  
  void addSubController(std::shared_ptr<BaseController> &c){
    subControllers_.push_back(c);
  }

  
  Controller<T>(){
    T::getInstance((T*)this);
  }

  
  Controller(T * controller) 
    : controller_(controller){}
    
  
  static Controller& getInstance( T *  target = nullptr){
    static Controller instance(target);
    return instance;
  }
  
  static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    T *t = (getInstance().controller_);
    t->keyCallbackImpl(window, key, scancode, action, mods);
    t->subCallbacks(window, key, scancode, action, mods);
  }    

  
  T getController(){
    return controller_;
  }

  private:
    T *controller_;
    std::vector<std::shared_ptr<BaseController> > subControllers_;
  };

}

#endif
