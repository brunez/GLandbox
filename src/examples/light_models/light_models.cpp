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
#include <glm/gtx/rotate_vector.hpp>

#include "gllog.h"
#include "init.h"

#include "light_models.h" 
#include "ShaderManager.h"
#include "ShaderDough.h"
#include "CameraController.h"
#include "tiny_obj_loader.h"
#include "glm_print.h"

int main(){       
  LightModels scene;      
  scene.start(); 
  return 0;
}

void LightModels::initialize(){

  std::shared_ptr<gland::BaseController> cameraController(new gland::CameraController(camera_, window_));
  controllers_.push_back(cameraController);  
  cameraController->generateHelpText(help_);


  /*
    shaderDough.addShaderFromFile("../src/examples/light_models/gouraud_vs.glsl", GL_VERTEX_SHADER);
    shaderDough.addShaderFromFile("../src/shaders/fragment01_fs.glsl", GL_FRAGMENT_SHADER);
  */

  gland::ShaderManager shaderManager;

  gland::ShaderDough shaderDough;    
  shaderDough.addShaderFromFile("../src/shaders/phong_vs.glsl", GL_VERTEX_SHADER);
  shaderDough.addShaderFromFile("../src/shaders/phong_fs.glsl", GL_FRAGMENT_SHADER);
  npPhong_ = shaderManager.cookDough(shaderDough);
  shaderPrograms_.push_back(npPhong_);

  gland::ShaderDough bpDough;    
  bpDough.addShaderFromFile("../src/shaders/blinnphong_vs.glsl", GL_VERTEX_SHADER);
  bpDough.addShaderFromFile("../src/shaders/blinnphong_fs.glsl", GL_FRAGMENT_SHADER);
  npBlinnPhong_ = shaderManager.cookDough(bpDough);
  shaderPrograms_.push_back(npBlinnPhong_);
      
  gland::ShaderDough gDough;    
  gDough.addShaderFromFile("../src/shaders/gouraud_vs.glsl", GL_VERTEX_SHADER);
  gDough.addShaderFromFile("../src/shaders/gouraud_fs.glsl", GL_FRAGMENT_SHADER);
  npGouraud_ = shaderManager.cookDough(gDough);
  shaderPrograms_.push_back(npGouraud_);
            
  //Add object
  std::string inputfile = "../res/sphere.obj";
  std::vector<tinyobj::shape_t> shapes;
  std::string err = tinyobj::LoadObj(shapes, inputfile.c_str());
  std::shared_ptr<gland::Body> obj(new gland::Body(shapes[0]));
  obj->setShader(npPhong_);
  standardObjects_.push_back(obj);

  gland::ShaderDough checkerDough;
  checkerDough.addShaderFromFile("../src/shaders/checker_vs.glsl", GL_VERTEX_SHADER);
  checkerDough.addShaderFromFile("../src/shaders/checker_fs.glsl", GL_FRAGMENT_SHADER);
  GLuint checkerProgram = shaderManager.cookDough(checkerDough);
  shaderPrograms_.push_back(checkerProgram);
  //Add plane
  inputfile = "../res/floor.obj";
  err = tinyobj::LoadObj(shapes, inputfile.c_str());
  std::shared_ptr<gland::Body> floor(new gland::Body(shapes[0]));
  floor->setShader(checkerProgram);
  floor->translate(0.0f, -5.0f, 0.0f);
  standardObjects_.push_back(floor);


  //glEnable (GL_CULL_FACE); // cull face
  glCullFace (GL_BACK); // cull back face
  //glFrontFace (GL_CW); // GL_CCW for counter clock-wise
            
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  camera_->lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}

//========================================
//      	  DRAW
//========================================

void LightModels::run(double time){
  const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
  const GLfloat depth[] = {1.0f};
  glClearBufferfv(GL_COLOR, 0, color);
  glClearBufferfv(GL_DEPTH, 0, depth);

  camera_->update();
  glm::mat4 projectionMatrix  = camera_->getProjectionMatrix();
  glm::mat4 viewMatrix  = camera_->getViewMatrix();           
  const float *viewSource = (const float*)glm::value_ptr(viewMatrix);
  const float *projectionSource = (const float*)glm::value_ptr(projectionMatrix);

  //Light in view space
  //      vLight = glm::rotate(vLight, 0.05f, glm::vec3(0.0f, 1.0f, 0.0f) );
  glm::vec3 vLightPos(10.0f, 2.0f, 0.0f);
  glm::vec3 vLight = glm::vec3(viewMatrix * glm::vec4(vLightPos, 1.0f) );
  const float *light = (const float*)glm::value_ptr(vLight);
      
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
    glUniformMatrix4fv (projLocation, 1, GL_FALSE, projectionSource);

    GLuint viewLocation = glGetUniformLocation (shaderProgram, "view");
    glUniformMatrix4fv (viewLocation, 1, GL_FALSE, viewSource);

    GLuint mvLocation = glGetUniformLocation (shaderProgram, "mvMatrix");
    glUniformMatrix4fv (mvLocation, 1, GL_FALSE, mvSource);

    GLuint normalMatrixLocation = glGetUniformLocation (shaderProgram, "normalMatrix");
    glUniformMatrix4fv (normalMatrixLocation, 1, GL_FALSE, normalMatrixSource);

    GLuint lightLocation = glGetUniformLocation (shaderProgram, "viewLightPos");
    if(lightLocation != 0){
      glUniform3fv (lightLocation, 1, light);
    } else {
      std::cerr << "Non-existent uniform" << std::endl;
    }

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
  }
}



 
