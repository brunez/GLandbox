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
 * File shadow_mapping.cpp
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
#include "init.h"

#include "shadow_mapping.h" 
#include "ShaderManager.h"
#include "ShaderDough.h"
#include "CameraController.h"
#include "SMController.h"
#include "tiny_obj_loader.h"
#include "glm_print.h"
#include "stuff.h"

int main(){

  ShadowMapping scene;      
  scene.start();
 
  return 0;
}

void ShadowMapping::initialize(){

  //controllers_.push_back(std::shared_ptr<gland::BaseController>(new gland::CameraController(camera_, window_)));
  std::shared_ptr<gland::BaseController> cameraController(new gland::CameraController(camera_, window_));
  std::shared_ptr<gland::BaseController> smController(new gland::SMController(params_, window_));
  controllers_.push_back(cameraController);  
  controllers_.push_back(smController);

  glfwSetKeyCallback(window_, &gland::Controller<gland::SMController>::keyCallback);

  params_["offsetFactor"] = gland::Param("Polygon offset factor");
  params_["offsetFactor"] = 4.0f;
  params_["offsetUnits"] = gland::Param("Polygon offset units");
  params_["offsetUnits"] = 4.0f;
  params_["renderDepth"] = gland::Param("Render depth");
  params_["renderDepth"] = false;
  params_["manualLight"] = gland::Param("Manual light");
  params_["manualLight"] = false;
  params_["angle"] = 0.0f;
  params_["lightSpeed"] = gland::Param("Light Rotation Speed");
  params_["lightSpeed"] = 1.0f;
  params_["lightRadius"] = gland::Param("Light circ. radius");
  params_["lightRadius"] = 10.0f;

  cameraController->generateHelpText(help_);
  smController->generateHelpText(help_);

  gland::ShaderDough shaderDough;
    
  shaderDough.addShaderFromFile("../src/shaders/shadowmapping_vs.glsl", GL_VERTEX_SHADER);
  shaderDough.addShaderFromFile("../src/shaders/shadowmapping_fs.glsl", GL_FRAGMENT_SHADER);

  gland::ShaderManager shaderManager;

  nViewProgram_ = shaderManager.cookDough(shaderDough);

  shaderPrograms_.push_back(nViewProgram_);

  //Light program
  gland::ShaderDough lightDough;
    
  lightDough.addShaderFromFile("../src/shaders/shadowmappinglight_vs.glsl", GL_VERTEX_SHADER);
  lightDough.addShaderFromFile("../src/shaders/shadowmappinglight_fs.glsl", GL_FRAGMENT_SHADER);

  nLightProgram_ = shaderManager.cookDough(lightDough);

  shaderPrograms_.push_back(nLightProgram_);

  //Depth program
  gland::ShaderDough depthDough;
    
  depthDough.addShaderFromFile("../src/shaders/shadowmappingdepth_vs.glsl", GL_VERTEX_SHADER);
  depthDough.addShaderFromFile("../src/shaders/shadowmappingdepth_fs.glsl", GL_FRAGMENT_SHADER);

  nDepthProgram_ = shaderManager.cookDough(depthDough);

  shaderPrograms_.push_back(nDepthProgram_);

  //Light source program
  gland::ShaderDough lightSourceDough;
  lightSourceDough.addShaderFromFile("../src/shaders/lightsource_vs.glsl", GL_VERTEX_SHADER);
  lightSourceDough.addShaderFromFile("../src/shaders/lightsource_fs.glsl", GL_FRAGMENT_SHADER);
  nLightSourceProgram_ = shaderManager.cookDough(lightSourceDough);
  shaderPrograms_.push_back(nLightSourceProgram_);

  //Frame buffer for depth
  glGenFramebuffers(1, &nDepthFbo_);
  glBindFramebuffer(GL_FRAMEBUFFER, nDepthFbo_);

  glGenTextures(1, &nDepthTex_);
  glBindTexture(GL_TEXTURE_2D, nDepthTex_);
  glTexStorage2D(GL_TEXTURE_2D, 11, GL_DEPTH_COMPONENT32F, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, nDepthTex_, 0);

  glGenTextures(1, &nDepthDebug_);
  glBindTexture(GL_TEXTURE_2D, nDepthDebug_);
  glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, nDepthDebug_, 0);

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);


  //VAO for drawing depth texture
  glGenVertexArrays(1, &nQuadVao_);
  glBindVertexArray(nQuadVao_);


  glEnable(GL_DEPTH_TEST);

  //Add object
  std::string inputfile = "../res/sphere.obj";
  std::vector<tinyobj::shape_t> shapes;
  std::string err = tinyobj::LoadObj(shapes, inputfile.c_str());
  std::shared_ptr<gland::Body> obj(new gland::Body(shapes[0]));
  obj->setShader(nViewProgram_);
  obj->translate(3.0f, 0.0f, -3.0f);
  standardObjects_.push_back(obj);

  //Add another sphere
  std::shared_ptr<gland::Body> sphere2(new gland::Body(shapes[0]));
  sphere2->setShader(nViewProgram_);
  sphere2->translate(-10.0f, -0.0f, -2.0f);
  standardObjects_.push_back(sphere2);

  inputfile = "../res/thickblock.obj";
  err = tinyobj::LoadObj(shapes, inputfile.c_str());
  std::shared_ptr<gland::Body> floor(new gland::Body(shapes[0]));
  floor->setShader(nViewProgram_);
  floor->rotate(0.0f, 1.0f, 0.0f, 0.0f);
  floor->translate(0.0f, 0.0f, -5.0f);
  standardObjects_.push_back(floor);

  inputfile = "../res/monkey.obj";
  err = tinyobj::LoadObj(shapes, inputfile.c_str());
  std::shared_ptr<gland::Body> monkey(new gland::Body(shapes[0]));
  monkey->setShader(nViewProgram_);
  monkey->translate(-2.0f, -0.0f, 0.0f);
  standardObjects_.push_back(monkey);

  inputfile = "../res/light.obj";
  err = tinyobj::LoadObj(shapes, inputfile.c_str());
  std::shared_ptr<gland::Body> lightSource(new gland::Body(shapes[0]));
  lightSource->setShader(nLightSourceProgram_);
  lightSources_.push_back(lightSource);

  glEnable (GL_CULL_FACE); // cull face
  glCullFace (GL_BACK); // cull back face
  //glFrontFace (GL_CW); // GL_CCW for counter clock-wise
            
  camera_->lookAt(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}

void ShadowMapping::run(double time){

  glEnable(GL_DEPTH_TEST);
  renderScene(true, time);   

  if (params_["renderDepth"]){
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, black);    
  const GLfloat depth[] = {1.0f};
  glClearBufferfv(GL_DEPTH, 0, depth);
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(nQuadVao_);
    glUseProgram(nDepthProgram_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, nDepthDebug_);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  }else{
    renderScene(false, time);
  }
}

void ShadowMapping::renderScene(bool fromLight, float time){

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

  float angle;
  if(params_["manualLight"]){
    angle = params_["angle"];
  } else {
    angle = (time - previousTime) * 0.5f;
  }

  //Move the light
  std::shared_ptr<gland::Body> lightSource = lightSources_.at(0);
  glm::vec3 vLightPos = glm::vec3(0.0f);
  glm::mat4 rot = lightSource->getRotationMatrix();
  glm::mat4 trm = glm::translate(glm::vec3(params_["lightRadius"], 0.0f, 0.0f));
  rot = glm::rotate(rot,  -sinf(angle*2e1f) * (params_["lightSpeed"]) , glm::vec3(0.0f, 1.0f, 0.0f));
  //We want a circular motion, so we first translate then rotate
  glm::mat4 transform = rot * trm;
  vLightPos = glm::vec3(transform * glm::vec4(vLightPos, 1.0f));
  trm = rot * trm;
  //And save the newly computed matrices in the light object
  lightSource->setTranslationMatrix(trm);
  lightSource->setRotationMatrix(rot);
      
  //Restore manuel rotation
  params_["angle"] = 0.0f;

  //Matrices from light's position for depth computation
  glm::mat4 lightViewMatrix = glm::lookAt(vLightPos, glm::vec3(-5.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 lightProjMatrix = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 200.0f);
  glm::mat4 lightVp = lightProjMatrix * lightViewMatrix;
  glm::mat4 scaleBiasMatrix = glm::mat4(glm::vec4(0.5f, 0.0f, 0.0f, 0.0f),
                                        glm::vec4(0.0f, 0.5f, 0.0f, 0.0f),
                                        glm::vec4(0.0f, 0.0f, 0.5f, 0.0f),
                                        glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

  //Light in view space, for rendering the source
  vLightPos = glm::vec3(viewMatrix*glm::vec4(vLightPos, 1.0f));
  const float *light = (const float*)glm::value_ptr(vLightPos);

  if(fromLight){
    //Render from light    
    glBindFramebuffer(GL_FRAMEBUFFER, nDepthFbo_);
    glViewport(0, 0, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(params_["offsetFactor"], params_["offsetUnits"]);
    glUseProgram(nLightProgram_);
    static const GLenum buffs[] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, buffs);
    glClearBufferfv(GL_COLOR, 0, zero);
  } else {
    int width, height;
    glfwGetWindowSize(window_, &width, &height);
    glViewport(0, 0, width, height);
    glClearBufferfv(GL_COLOR, 0, gray);
    glUseProgram(nViewProgram_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, nDepthTex_);
    GLuint projLocation = glGetUniformLocation (shaderPrograms_.at(0), "proj");          
    glUniformMatrix4fv (projLocation, 1, GL_FALSE, projectionSource);
    glDrawBuffer(GL_BACK);   
  }

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

    glm::mat4 shadowSbpvMatrix =  scaleBiasMatrix * lightProjMatrix * lightViewMatrix * modelMatrix;
    const float *shadowSource = (const float*)glm::value_ptr(shadowSbpvMatrix);

    if(fromLight){      
      
        glUseProgram (nLightProgram_);

        const float *lightVpSource = (const float*)glm::value_ptr(lightVp);      
        GLuint lightVpLocation = glGetUniformLocation (nLightProgram_, "vp");
        if(lightVpLocation >= 0){
          glUniformMatrix4fv(lightVpLocation, 1, GL_FALSE, lightVpSource);
        } else {
          std::cerr << "Non-existent uniform: vp" << std::endl;
        }

        GLuint modelLocation = glGetUniformLocation (nLightProgram_, "model");
        if(modelLocation >= 0){
          glUniformMatrix4fv(modelLocation, 1, GL_FALSE, mSource);
        } else {
          std::cerr << "Non-existent uniform: model" << std::endl;
        }
      

    } else {
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

      GLuint shadowMatrixLocation = glGetUniformLocation (shaderProgram, "shadowMatrix");
      if(shadowMatrixLocation >= 0){
        glUniformMatrix4fv (shadowMatrixLocation, 1, GL_FALSE, shadowSource);
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
  }

  if(!fromLight){            

    for(std::shared_ptr<gland::Body> b : lightSources_){

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
        glUniform1f(stepLocation, time-previousTime);
      } else {
        std::cerr << "Non-existent uniform" << std::endl;
      }
           
      glBindVertexArray(b->getVaoName());
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b->getIndexBufferName());
      //glDrawArrays (GL_TRIANGLES, 0, b->getVertexCount());
            glDrawElements(
                     GL_TRIANGLES,      // mode
                     b->getIndices().size(),    // count
                     GL_UNSIGNED_INT,   // type
                     (void*)0           // element array buffer offset
                     );
      
      glBindVertexArray(0);
    }
  }

  if(fromLight){      
    glDisable(GL_POLYGON_OFFSET_FILL);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  } else {
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }

  //========================================

  
}

 
