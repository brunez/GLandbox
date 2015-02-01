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
 * File Scene.cpp
 * Author: bruno
 */

#include "Scene.h" 
#include <iostream>

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <ftglyph.h>

#include "ShaderManager.h"
#include "ShaderDough.h"
#include "CameraController.h"

namespace gland{

  Scene::Scene():
    camera_(new gland::Camera),
    showHelp_(true){
    window_ = gland::init();
    
  }

  Scene::~Scene(){    
    //clean up
    for( GLuint p : shaderPrograms_){
      glDeleteProgram(p);
    }
    for(GLuint i : vaoList){
      glDeleteVertexArrays(1, &i);
    }
    glfwDestroyWindow(window_);
    glfwTerminate();
  }

  void Scene::start(){

    //Params program
    gland::ShaderManager shaderManager;
    gland::ShaderDough paramsDough;  
    paramsDough.addShaderFromFile("../src/shaders/text_vs.glsl", GL_VERTEX_SHADER);
    paramsDough.addShaderFromFile("../src/shaders/text_fs.glsl", GL_FRAGMENT_SHADER);
    npParams_ = shaderManager.cookDough(paramsDough);
    shaderPrograms_.push_back(npParams_);

    glActiveTexture(GL_TEXTURE1);
    glGenTextures(1, &ntParams_);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glBindTexture(GL_TEXTURE_2D, ntParams_);
    //  glUniform1i(uniform_tex, 0);
  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenVertexArrays(1, &nvParams_);
    glBindVertexArray(nvParams_);
    glGenBuffers(1, &nbParams_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, nbParams_);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    if(FT_Init_FreeType(&ft)) {
      fprintf(stderr, "Could not init freetype library\n");
    }
    if(FT_New_Face(ft, "../res/FreeSans.ttf", 0, &face)) {
      fprintf(stderr, "Could not open font\n");
    }

    g = face->glyph;

    FT_Set_Pixel_Sizes(face, 0, 24);
    
    this->initialize();
    this->loop();
    
  }


  void Scene::loop(){

    while (!glfwWindowShouldClose (window_)) {
      double time = glfwGetTime();
      this->run(time);
      this->renderOverlays();

      //TODO To window controller
      glfwPollEvents ();
      if (GLFW_PRESS == glfwGetKey (window_, GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose (window_, 1);
      }
      if (GLFW_PRESS == glfwGetKey (window_, GLFW_KEY_F1)){
        showHelp_ = !showHelp_;
      }
      for(std::shared_ptr<gland::BaseController> controller : controllers_){
        controller->handleInput();
      }

      GLuint err;
      unsigned int errorCount = 0;
      while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
        std::cerr << gluErrorString(err) << std::endl;
        errorCount++;
      }
      if(errorCount > 0){
        std::cout << "--- " << errorCount << " error" << (errorCount ==1?"":"s")  << std::endl;
      }
      glfwSwapBuffers(window_);
      this->doPostWork();
      previousTime = time;
    }
  }


  void Scene::renderOverlays(){

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glEnable (GL_BLEND); 
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glActiveTexture(GL_TEXTURE1);

    glBindTexture(GL_TEXTURE_2D, ntParams_);

    this->renderParams();
    
    this->renderHelp();
    
    //Release bound objects
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Restore pack alignment
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
  }

  void Scene::renderParams(){
    std::map<std::string, gland::Param>::iterator it;
    float x = -0.98f;
    float y = 0.92f;
    
    for(it = params_.begin(); it != params_.end(); ++it){
      //TODO clear buffer, rather than build it every time
      std::ostringstream buff;
      buff << it->second;
      std::string text = it->second.name_ + ": " + buff.str();
      this->renderText(text, x, y);
      y -= 0.08f;
    }

  }

  void Scene::renderHelp(){
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    
    float sx = 2.0 / static_cast<float>(width);    
    float sy = 2.0 / static_cast<float>(height);    

    std::string text = "Press F1 for help";
    if(!showHelp_){
      this->renderText(text, 1.0f-g->bitmap.width * sx * text.size(), 0.92f);
    } else {
      std::vector<std::string>::iterator it;
      float y = 0.92f;
      for(it = help_.begin(); it != help_.end(); it++){
        std::string text = *it;
        float width = 0.0f;
        float height = g->bitmap.rows * sy;
        //TODO too costly? Just for debugging, anyway
        for(const char *p = text.c_str(); *p; p++){
          FT_Load_Char(face, *p, FT_LOAD_RENDER);          
          width += (g->advance.x >> 6) * sx;
          if(g->bitmap.rows * sy > height){
            height = g->bitmap.rows * sy;
          }
        }
        float x = 1.0f - width - 0.02f;
        this->renderText(text, x, y);
        y -= height*1.5f;
      }
    }

  }

  void Scene::renderText(std::string text, float x, float y){
    int width, height;
    glfwGetWindowSize(window_, &width, &height);

    float sx = 2.0 / static_cast<float>(width);    
    float sy = 2.0 / static_cast<float>(height);    

      const char *p;    
      for(p = text.c_str(); *p; p++){
         int success = FT_Load_Char(face, *p, FT_LOAD_RENDER);
        if(success != 0){
          std::cout << "Error loading char: " << success << std::endl;   
        }
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, g->bitmap.width, g->bitmap.rows, 0, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);
        float x2 = x + g->bitmap_left * sx;
        float y2 = -y - g->bitmap_top * sy;
        float w = g->bitmap.width * sx;
        float h = g->bitmap.rows * sy;
        GLfloat box[4][4] = {
          {x2,     -y2    , 0, 0},
          {x2 + w, -y2    , 1, 0},
          {x2,     -y2 - h, 0, 1},
          {x2 + w, -y2 - h, 1, 1},
        };

        glUseProgram(npParams_);
        glBindVertexArray(nvParams_);
        glBindTexture(GL_TEXTURE_2D, ntParams_);
        glBindBuffer(GL_ARRAY_BUFFER, nbParams_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        x += (g->advance.x >> 6) * sx;
        y += (g->advance.y >> 6) * sy;
      }
  }

}


 

