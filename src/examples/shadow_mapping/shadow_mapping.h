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
 * File shadow_mapping.h
 * Author: bruno
 */

#ifndef SHADOW_MAPPING_H
#define SHADOW_MAPPING_H

#include "Scene.h"

class ShadowMapping : public gland::Scene{
 public:
  void initialize();
  void run(double time);
  void renderScene(bool fromLight, float step);
 private:
  GLuint nViewProgram_;
  GLuint nDepthProgram_;
  GLuint nLightProgram_;
  GLuint nLightSourceProgram_;
  GLuint nDepthFbo_;
  GLuint nDepthTex_;
  GLuint nDepthDebug_;
  GLuint nQuadVao_;
  std::vector<std::shared_ptr<gland::Body>> lightSources_;
  const int DEPTH_TEXTURE_SIZE = 4096;
};




#endif
