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
 * File TextureLoader.h
 * Author: bruno
 */

#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <png.h>
#include <GL/glew.h>

namespace gland{

  struct TextureData{
    png_byte *data;
    int width;
    int height;
    GLint format;

    TextureData(){}
  TextureData(png_byte *data, int width, int height, GLint format):data(data), width(width), height(height), format(format){}
        
  };

  class TextureLoader {
  public:
    TextureLoader();

    GLuint loadFromPng(const char * file_name);
    GLuint loadFromPng_(const char * file_name);

    TextureData loadDataFromPng(const char * file_name);

  private:
  };

}
#endif //TEXTURELOADER_H
