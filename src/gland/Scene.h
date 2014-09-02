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
 * File Scene.h
 * Author: bruno
 */

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "Camera.h"
#include "Body.h"
#include "init.h"
#include "Param.h"


namespace gland{

  class BaseController;

  /**
   * This class represents a basic OpenGL scene. It also manages GLFW resource initialization and clean-up.
   */
  class Scene {
  public:

    explicit Scene();
    virtual ~Scene();
    Scene & operator =(const Scene &) = delete;
    Scene(const Scene&) = delete;
    void start();
    void loop();
    virtual void initialize() = 0;
    virtual void run(double time) = 0;
    void renderOverlays();
    void renderParams();
    void renderHelp();
    void renderText(std::string text, float x, float y);
    virtual void doPostWork(){}
    

  protected:
    /**
     * The camera object for this scene
     */
    std::shared_ptr<gland::Camera> camera_;

    /**
     * The controllers for this scene. This objects handle input.
     */
    std::vector<std::shared_ptr<gland::BaseController> > controllers_;

    /**
     * The GLFW window
     */
    GLFWwindow* window_;

    /**
     * A vector of basic OpenGL objects to be shown in the scene
     */
    std::vector<std::shared_ptr<gland::Body>> standardObjects_;
    
    /**
     * The list of shaders used in this scene
     */
    std::vector<GLuint> shaderPrograms_;

    /**
     * The list of VAOs used in this scene
     */
    std::vector<GLuint> vaoList;

    /**
     * Parameters for debugging
     */
    std::map<std::string, gland::Param> params_;

    /**
     * Help messages
     */
    std::vector<std::string> help_;

    double previousTime;

  private:
    
    /**
     * OpenGL names for rendering parameters
     */
    GLuint npParams_;
    GLuint ntParams_;
    GLuint nbParams_;
    GLuint nvParams_;
    FT_Library ft;  
    FT_Face face;    
    FT_GlyphSlot g;
    bool showHelp_;

  };
}




#endif //SCENE_H
