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
 * File fog.cpp
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
#include <glm/gtc/matrix_transform.hpp>

#include "gllog.h"

#include "fog.h" 
#include "ShaderManager.h"
#include "ShaderDough.h"
#include "CameraController.h"
#include "tiny_obj_loader.h"
#include "glm_print.h"
#include "stuff.h"
#include "Controller.h"

int main(){
  Fog scene;      
  scene.start();  
  return 0;
}

void Fog::initialize(){

  std::shared_ptr<gland::BaseController> cameraController(new gland::CameraController(camera_, window_));
  controllers_.push_back(cameraController);  
  cameraController->generateHelpText(help_);
  
  gland::ShaderManager shaderManager;

  //Light program
  gland::ShaderDough lightDough;
    
  lightDough.addShaderFromFile("../src/shaders/phong_vs.glsl", GL_VERTEX_SHADER);
  lightDough.addShaderFromFile("../src/shaders/phong_fs.glsl", GL_FRAGMENT_SHADER);
  nLightProgram_ = shaderManager.cookDough(lightDough);
  shaderPrograms_.push_back(nLightProgram_);

  //Add object
  std::string inputfile = "../res/sphere.obj";
  std::vector<tinyobj::shape_t> shapes;
  std::string err = tinyobj::LoadObj(shapes, inputfile.c_str());
  std::shared_ptr<gland::Body> obj(new gland::Body(shapes[0]));
  obj->setShader(nLightProgram_);
  standardObjects_.push_back(obj);

  //glEnable (GL_CULL_FACE); // cull face
  glCullFace (GL_BACK); // cull back face
  //glFrontFace (GL_CW); // GL_CCW for counter clock-wise
            
  camera_->lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}

void Fog::run(double time){

  gllog::update_fps_counter (window_);
      
  const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
  const GLfloat depth[] = {1.0f};

  static const GLfloat ones[] = { 1.0f };
  static const GLfloat zero[] = { 0.0f };
  static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
  
  camera_->update();

  glm::mat4 projectionMatrix  = camera_->getProjectionMatrix();
  glm::mat4 viewMatrix  = camera_->getViewMatrix();           
  const float *viewSource = (const float*)glm::value_ptr(viewMatrix);
  const float *projectionSource = (const float*)glm::value_ptr(projectionMatrix);

  glm::vec3 vLightPos = glm::vec3(10.0f, 3.0f, 0.0f);
      
  //Light in view space, for rendering the source
  vLightPos = glm::vec3(viewMatrix*glm::vec4(vLightPos, 1.0f));
  const float *light = (const float*)glm::value_ptr(vLightPos);
  int width, height;
  glfwGetWindowSize(window_, &width, &height);
  glViewport(0, 0, width, height);
  glClearBufferfv(GL_COLOR, 0, gray);
  glClearBufferfv(GL_DEPTH, 0, ones);
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

    GLuint shaderProgram = b->getShader();
    glUseProgram (shaderProgram);    
    GLuint projLocation = glGetUniformLocation (shaderProgram, "proj");
    if(projLocation >= 0){
      glUniformMatrix4fv (projLocation, 1, GL_FALSE, projectionSource);
    } else {
      std::cerr << "Non-existent uniform" << std::endl;
    }

    GLuint viewLocation = glGetUniformLocation (shaderProgram, "view");
    if(viewLocation >= 0){
      glUniformMatrix4fv (viewLocation, 1, GL_FALSE, viewSource);
    } else {
      std::cerr << "Non-existent uniform" << std::endl;
    }

    GLuint mvLocation = glGetUniformLocation (shaderProgram, "mvMatrix");
    if(mvLocation >= 0){
      glUniformMatrix4fv (mvLocation, 1, GL_FALSE, mvSource);
    } else {
      std::cerr << "Non-existent uniform" << std::endl;
    }

    GLuint normalMatrixLocation = glGetUniformLocation (shaderProgram, "normalMatrix");
    if(normalMatrixLocation >= 0){
      glUniformMatrix4fv (normalMatrixLocation, 1, GL_FALSE, normalMatrixSource);
    } else {
      std::cerr << "Non-existent uniform" << std::endl;
    }

    GLuint lightLocation = glGetUniformLocation (shaderProgram, "viewLightPos");
    if(lightLocation >= 0){
      glUniform3fv (lightLocation, 1, light);
    } else {
      std::cerr << "Non-existent uniform" << std::endl;
    }

    GLuint stepLocation = glGetUniformLocation (shaderProgram, "step");
    if(stepLocation >= 0){
      glUniform1f(stepLocation, time - previousTime);
    } else {
      std::cerr << "Non-existent uniform" << std::endl;
    }
   
    glBindVertexArray(b->getVaoName());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b->getIndexBufferName());
    glDrawElements(
                   GL_TRIANGLES,      // mode
                   b->getIndices().size(),    // count
                   GL_UNSIGNED_INT,   // type
                   (void*)0           // element array buffer offset
                   );
    
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
}
