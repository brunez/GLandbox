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

#include <iostream>
#include "Body.h"

namespace gland{

  Body::Body():
    nPositions_(0),
    nNormals_(0),
    nIndices_(0),
    nVao_(0),
    nShader_(0){
  }
  
  Body::~Body(){
        
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if(nPositions_ != 0){
      glDeleteBuffers(1, &nPositions_);
    }
    if(nIndices_ != 0){
      glDeleteBuffers(1, &nIndices_);
    }
    if(nNormals_ != 0){
      glDeleteBuffers(1, &nNormals_);
    }

    glBindVertexArray(0);
    
    if(nVao_ != 0){
      glDeleteVertexArrays(1, &nVao_);
    
    }
    
    delete [] positionData;
    delete [] normalData;
    delete [] indexData;    
        
  }

  Body::Body(tinyobj::shape_t &shape){

    //Retrieve positions
    for (size_t v = 0; v < shape.mesh.positions.size() / 3; v++) {
      this->addVertex(shape.mesh.positions[3*v+0],
                      shape.mesh.positions[3*v+1],
                      shape.mesh.positions[3*v+2]);
    }
    
    positionData = new float[positions_.size()];    
    //positions_ = shape.mesh.positions;
    std::copy(positions_.begin(), positions_.end(), positionData);

    normalData = new float[shape.mesh.normals.size()];
    normals_ = shape.mesh.normals;
    std::copy(normals_.begin(), normals_.end(), normalData);

    for (size_t f = 0; f < shape.mesh.indices.size(); f++) {
      this->addIndex(shape.mesh.indices[f]);
    }


    indexData = new unsigned int[indices_.size()];
    //indices_ = shape.mesh.indices;
    std::copy(indices_.begin(), indices_.end(), indexData);

    glGenVertexArrays(1, &nVao_);
    glBindVertexArray(nVao_);
    //Positions
    positionCount = positions_.size();
    if(positionCount > 0){
      glGenBuffers(1, &nPositions_);
      glBindBuffer(GL_ARRAY_BUFFER, nPositions_);
      glBufferData(GL_ARRAY_BUFFER, sizeof(*positionData)*positionCount, positionData, GL_STATIC_DRAW);
      glEnableVertexAttribArray (0);
      glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    }

    //Normals
    normalCount = normals_.size();
    if(normalCount > 0){
      glGenBuffers(1, &nNormals_);
      glBindBuffer(GL_ARRAY_BUFFER, nNormals_);
      glBufferData(GL_ARRAY_BUFFER, sizeof(*normalData)*normalCount, normalData, GL_STATIC_DRAW);  
      glEnableVertexAttribArray (1);
      glVertexAttribPointer (1, 3, GL_FLOAT, GL_TRUE, 0, NULL);
    }

    //Indices
    indexCount = indices_.size();
    if(indexCount > 0){
      glGenBuffers(1, &nIndices_);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, nIndices_);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                   sizeof(*indexData)*indexCount,
                   indexData,
                   GL_STATIC_DRAW);
    }

    
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindVertexArray(0);
    
  }

  void Body::addVertex(float x, float y, float z){
    positions_.push_back(x);
    positions_.push_back(y);
    positions_.push_back(z);
  }

  void Body::addIndex(int index){
    indices_.push_back(index);
  }

  float* Body::getVertexArray(){
    float data[positions_.size()];
    std::copy(positions_.begin(), positions_.end(), data);
    return data;
  }
    
  int Body::getVertexCount(){ 
    return positions_.size()/3;
  }
  std::vector<float> Body::getValues(){
    return positions_;
  }

  std::vector<unsigned int> Body::getIndices(){
    return indices_;
  }

  GLuint Body::getVaoName(){
    return nVao_;
  }

  GLuint Body::getPositionsBufferName(){
    return nPositions_;
  }

  GLuint Body::getIndexBufferName(){
    return nIndices_;
  }

  GLuint Body::getNormalsBufferName(){
    return nNormals_;
  }

  int Body::getPositionCount(){
    return positionCount;
  }

  int Body::getNormalCount(){
    return normalCount;
  }

  int Body::getIndexCount(){
    return indexCount;
  }

  void Body::setShader(GLuint shader){
    nShader_ = shader;
  }

  GLuint Body::getShader(){
    return nShader_;
  }

  std::ostream &operator<< (std::ostream &out, const Body &body) {
    out << "========================================" 
        
        << "Work in progress" << std::endl;
    
    return out;
  }


}
