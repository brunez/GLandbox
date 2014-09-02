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
 * File light_models.h
 * Author: bruno
 */

#ifndef LIGHT_MODELS_H
#define LIGHT_MODELS_H

#include "Scene.h"

class LightModels : public gland::Scene{
 public:
  void initialize() override;
  void run(double time) override;
 private:
  GLuint npPhong_;
  GLuint npBlinnPhong_;
  GLuint npGouraud_;
};

#endif
