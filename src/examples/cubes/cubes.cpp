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
 * File cubes.cpp
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

#include "cubes.h" 
#include "ShaderManager.h"
#include "ShaderDough.h"
#include "CameraController.h"
#include "tiny_obj_loader.h"
#include "stuff.h"
#include "Controller.h"

int main(){
  Cubes scene;      
  scene.start();  
  return 0;
}

void Cubes::initialize(){

  std::shared_ptr<gland::BaseController> cameraController(new gland::CameraController(camera_, window_));
  controllers_.push_back(cameraController);  
  cameraController->generateHelpText(help_);

  gland::ShaderDough shaderDough;
  shaderDough.addShaderFromFile("../src/shaders/wave_vs.glsl", GL_VERTEX_SHADER);
  shaderDough.addShaderFromFile("../src/shaders/noise_fs.glsl", GL_FRAGMENT_SHADER);
  gland::ShaderManager shaderManager;
  GLuint shaderProgram = shaderManager.cookDough(shaderDough);
  shaderPrograms_.push_back(shaderProgram);
  
  /*  glGenVertexArrays(CUBE_COUNT, vao);
  glGenBuffers(CUBE_COUNT, nCubeBuffer);
  std::vector<float> values = gland::loadRaw("../res/raw/cube.raw");
  float data[values.size()];
  std::copy(values.begin(), values.end(), data);
  
  for(int i = 0; i < CUBE_COUNT; i++){
    for(int j = 0; j < values.size(); j+=3){
      data[j] += 0.55f;
      if(i % 100 == 0 && i > 0){
        data[j+2] -= 0.55f;
        data[j] -= 100.0f*0.55f;
      }
    }
    glBindVertexArray(vao[i]);
    glBindBuffer(GL_ARRAY_BUFFER, nCubeBuffer[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);                
  }   
  */  



  //Add objects
  std::string inputfile = "../res/cube.obj";
  std::vector<tinyobj::shape_t> shapes;
  std::string err = tinyobj::LoadObj(shapes, inputfile.c_str());
  float x = 0.0f;
  float z = 0.0f;
  for(int i = 0; i < CUBE_COUNT; i++){
    std::shared_ptr<gland::Body> obj(new gland::Body(shapes[0]));
    obj->setShader(shaderProgram);
    obj->translate(x, 0.0f, z);
    x += 2.5f;
    if(x >= 25.0f){
      x = 0.0f;
      z += 2.5f;
    }
    
    standardObjects_.push_back(obj);
  }

  //glEnable (GL_CULL_FACE); // cull face
  //glCullFace (GL_BACK); // cull back face
  //glFrontFace (GL_CW); // GL_CCW for counter clock-wise
            
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  camera_->lookAt(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}


void Cubes::run(double time){

  camera_->update();

  glm::mat4 viewMatrix  = camera_->getViewMatrix();
  glm::mat4 projectionMatrix  = camera_->getProjectionMatrix();

  //TODO Manage elsewhere
  int width;
  int height;
  glfwGetWindowSize (window_, &width, &height);
  glViewport (0, 0, width, height);

  //Retrieve the camera_ matrices
  //TODO Is it safe to use this array, or should I create a
  //fixed-size one?
  const float *viewSource = (const float*)glm::value_ptr(viewMatrix);
  const float *projectionSource = (const float*)glm::value_ptr(projectionMatrix);

  const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
  const GLfloat depth[] = {1.0f};

  glClearBufferfv(GL_COLOR, 0, color);
  glClearBufferfv(GL_DEPTH, 0, depth);

  gllog::update_fps_counter (window_);
                
  //========================================
  // Draw cubes
  //========================================


  for(std::shared_ptr<gland::Body> b : standardObjects_){

      glm::mat4 modelMatrix  = b->getModelMatrix();
      glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;

      //TODO Is it safe to use this array, or should I create a
      //fixed-size one?
      const float *mSource = (const float*)glm::value_ptr(modelMatrix);
      const float *mvSource = (const float*)glm::value_ptr(modelViewMatrix);

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

      GLuint stepLocation = glGetUniformLocation (shaderProgram, "step");
      if(stepLocation >= 0){
        glUniform1f(stepLocation, time);
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
  
  /*                
  GLuint shaderProgram = shaderPrograms_.at(0);
  glUseProgram (shaderProgram);    

  GLuint projLocation = glGetUniformLocation (shaderProgram, "proj");
  glUniformMatrix4fv (projLocation, 1, GL_FALSE, projectionSource);

  GLuint viewLocation = glGetUniformLocation (shaderProgram, "view");
  glUniformMatrix4fv (viewLocation, 1, GL_FALSE, viewSource);

  GLuint stepLocation = glGetUniformLocation (shaderProgram, "step");
  glUniform1f(stepLocation, time);
                
  for(int i = 0; i < CUBE_COUNT; i++){
    glBindVertexArray(vao[i]);
    glDrawArrays (GL_TRIANGLES, 0, 36);
  }
  */
}
