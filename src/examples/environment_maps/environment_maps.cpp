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
 * File environment_maps.cpp
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
#include <glm/gtx/rotate_vector.hpp>

#include "gllog.h"

#include "environment_maps.h" 
#include "ShaderManager.h"
#include "ShaderDough.h"
#include "stuff.h"
#include "CameraController.h"
#include "tiny_obj_loader.h"
#include "glm_print.h"

int main(){
  EnvMaps scene;     
  scene.start();
  return 0;
}

void EnvMaps::initialize(){

  std::shared_ptr<gland::BaseController> cameraController(new gland::CameraController(camera_, window_));
  controllers_.push_back(cameraController);  
  cameraController->generateHelpText(help_);

  gland::ShaderDough shaderDough;
    
  shaderDough.addShaderFromFile("../src/shaders/envmaps_vs.glsl", GL_VERTEX_SHADER);
  shaderDough.addShaderFromFile("../src/shaders/envmaps_fs.glsl", GL_FRAGMENT_SHADER);
  //shaderDough.addShaderFromFile("../src/shaders/eq_envmaps_fs.glsl", GL_FRAGMENT_SHADER);
  gland::ShaderManager shaderManager;

  GLuint shaderProgram = shaderManager.cookDough(shaderDough);

  shaderPrograms_.push_back(shaderProgram);
  //Add object
  std::string inputfile = "../res/sphere.obj";
  std::vector<tinyobj::shape_t> shapes;
  std::string err = tinyobj::LoadObj(shapes, inputfile.c_str());
  std::shared_ptr<gland::Body> obj(new gland::Body(shapes[0]));
  obj->setShader(shaderProgram);
  standardObjects_.push_back(obj);

  glActiveTexture(GL_TEXTURE0);
  gland::TextureLoader tl;        
  nTexture_ = tl.loadFromPng("../res/envmaps/window.png");

  glEnable (GL_CULL_FACE); // cull face
  glCullFace (GL_BACK); // cull back face
  //glFrontFace (GL_CW); // GL_CCW for counter clock-wise

  camera_->lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}

//========================================
//      	  DRAW
//========================================

void EnvMaps::run(double time){
  
  const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
  const GLfloat depth[] = {1.0f};
  
  glClearBufferfv(GL_COLOR, 0, color);
  glClearBufferfv(GL_DEPTH, 0, depth);
    
  camera_->update();
  glm::mat4 projectionMatrix  = camera_->getProjectionMatrix();
  glm::mat4 viewMatrix  = camera_->getViewMatrix();           
  const float *viewSource = (const float*)glm::value_ptr(viewMatrix);
  const float *projectionSource = (const float*)glm::value_ptr(projectionMatrix);
    
  //==============================
  //Draw standard objects
  //==============================
  for(std::shared_ptr<gland::Body> b : standardObjects_){
    glm::mat4 modelMatrix  = b->getModelMatrix();
    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
    glm::mat4 normalMatrix = glm::transpose(glm::inverse(modelViewMatrix));
      
    //TODO Is it safe to use this array, or should I create a
    //fixed-size one?
    const float *mSource = (const float*)glm::value_ptr(modelMatrix);
    const float *mvSource = (const float*)glm::value_ptr(modelViewMatrix);
    const float *normalMatrixSource = (const float*)glm::value_ptr(normalMatrix);

    glActiveTexture(GL_TEXTURE0);
      
    glBindTexture(GL_TEXTURE_2D, nTexture_);
      
    GLuint shaderProgram = b->getShader();
    glUseProgram (shaderProgram);
    GLuint projLocation = glGetUniformLocation (shaderProgram, "proj");
    glUniformMatrix4fv (projLocation, 1, GL_FALSE, projectionSource);
      
    GLuint viewLocation = glGetUniformLocation (shaderProgram, "view");
    glUniformMatrix4fv (viewLocation, 1, GL_FALSE, viewSource);
      
    GLuint mvLocation = glGetUniformLocation (shaderProgram, "mvMatrix");
    glUniformMatrix4fv (mvLocation, 1, GL_FALSE, mvSource);
      
    GLuint normalMatrixLocation = glGetUniformLocation (shaderProgram, "normalMatrix");
    glUniformMatrix4fv (normalMatrixLocation, 1, GL_FALSE, normalMatrixSource);
      
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
      
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
  }
 
}



 
