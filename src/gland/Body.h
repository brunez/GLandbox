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

#ifndef BODY_H
#define BODY_H

#include <vector>
#include <memory>

#include <GL/glew.h>

#include "tiny_obj_loader.h"
#include "Object.h"

namespace gland{

  class Body : public gland::Object{

  public:

    explicit Body();
    virtual ~Body();

    //TODO Force shader set?
    /**
     * Builds a Body from a tinyobj shape loaded from an OBJ file
     */
    explicit Body(tinyobj::shape_t &shape);

    void render();

    void addVertex(float x, float y, float z);
    void addIndex(int index);

    float* getVertexArray();
    int getVertexCount();
    std::vector<float> getValues();
    std::vector<unsigned int> getIndices();

    GLuint getVaoName();
    GLuint getPositionsBufferName();
    GLuint getIndexBufferName();
    GLuint getNormalsBufferName();

    int getPositionCount();
    int getNormalCount();
    int getIndexCount();

    void setShader(GLuint shader);
    GLuint getShader();

  private:
    std::vector<float> positions_;
    std::vector<unsigned int> indices_;
    std::vector<float> normals_;

    std::shared_ptr<gland::Material> material_;

    /**
     * Buffer names
     */
    GLuint nPositions_;
    GLuint nNormals_;
    GLuint nIndices_;

    float *positionData;
    float *normalData;
    unsigned int *indexData;

    /**
     * VAO name
     */
    GLuint nVao_;

    /**
     * Buffer counts
     */
    int positionCount;
    int indexCount;
    int normalCount;

    /**
     * Shader to be used to render this object
     */
    GLuint nShader_;


  };

  std::ostream &operator<< (std::ostream &out, const Body &body);

}
#endif
