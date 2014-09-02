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
 * File light_models.cpp
 * Author: bruno
 */

#include <iostream>
#include "TextureLoader.h"

#include <GL/glew.h>
#define GLFW_DLL
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gllog.h"
#include "init.h"

#include "bump_mapping.h" 
#include "ShaderManager.h"
#include "ShaderDough.h"
#include "stuff.h"
#include "CameraController.h"
#include "tiny_obj_loader.h"

int main(){
  BumpMapping scene;
  scene.start();
  return 0;
}

void BumpMapping::initialize(){

  std::shared_ptr<gland::BaseController> cameraController(new gland::CameraController(camera_, window_));
  controllers_.push_back(cameraController);  
  cameraController->generateHelpText(help_);

  gland::ShaderDough shaderDough;

  /*  
      shaderDough.addShaderFromFile("../src/examples/bump_mapping/phong_vs.glsl", GL_VERTEX_SHADER);
      shaderDough.addShaderFromFile("../src/examples/bump_mapping/phong_fs.glsl", GL_FRAGMENT_SHADER);
  */
  
  shaderDough.addShaderFromFile("../src/shaders/bump_mapping_vs.glsl", GL_VERTEX_SHADER);
  shaderDough.addShaderFromFile("../src/shaders/bump_mapping_fs.glsl", GL_FRAGMENT_SHADER);
  
  gland::ShaderManager shaderManager;

  GLuint shaderProgram = shaderManager.cookDough(shaderDough);

  shaderPrograms_.push_back(shaderProgram);
  
  std::string inputfile = "../res/plane.obj";
  std::vector<tinyobj::shape_t> shapes;
  std::string err = tinyobj::LoadObj(shapes, inputfile.c_str());
  std::shared_ptr<gland::Body> obj(new gland::Body(shapes[0]));
  obj->setShader(shaderProgram);
  standardObjects_.push_back(obj);

  
  glEnable (GL_CULL_FACE); // cull face
  glCullFace (GL_BACK); // cull back face
  //glFrontFace (GL_CW); // GL_CCW for counter clock-wise
            
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  camera_->lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}

//========================================
//      	  DRAW
//========================================

void BumpMapping::run(double time){

  camera_->update();
      
  //TODO Manage elsewhere
  int width;
  int height;
  glfwGetWindowSize (window_, &width, &height);
  glViewport (0, 0, width, height);


  const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
  const GLfloat depth[] = {1.0f};
  glClearBufferfv(GL_COLOR, 0, color);
  glClearBufferfv(GL_DEPTH, 0, depth);

  gllog::update_fps_counter (window_);
                
  glm::mat4 viewMatrix  = camera_->getViewMatrix();
  glm::mat4 projectionMatrix  = camera_->getProjectionMatrix();

  const float *projectionSource = (const float*)glm::value_ptr(projectionMatrix);
                
  for(std::shared_ptr<gland::Body> b : standardObjects_){
    glm::mat4 modelMatrix  = b->getModelMatrix();
    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
    const float *mvSource = (const float*)glm::value_ptr(modelViewMatrix);

    GLuint shaderProgram = b->getShader();
    glUseProgram (shaderProgram);    
    GLuint projLocation = glGetUniformLocation (shaderProgram, "proj");
    glUniformMatrix4fv (projLocation, 1, GL_FALSE, projectionSource);
    GLuint mvLocation = glGetUniformLocation (shaderProgram, "mvMatrix");
    glUniformMatrix4fv (mvLocation, 1, GL_FALSE, mvSource);

    GLuint stepLocation = glGetUniformLocation (shaderProgram, "step");
    glUniform1f(stepLocation, time);
    glBindVertexArray(b->getVaoName());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b->getIndexBufferName());
    //glDrawArrays (GL_TRIANGLES, 0, e.getVertexCount());
    glDrawElements(
                   GL_TRIANGLES,      // mode
                   b->getIndices().size(),    // count
                   GL_UNSIGNED_INT,   // type
                   (void*)0           // element array buffer offset
                   );
  }
}



 
