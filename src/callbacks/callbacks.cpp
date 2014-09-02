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

#include "callbacks.h"

namespace gllog{
#define GL_LOG_FILE "gl.log"
// a call-back function
void glfw_window_size_callback (GLFWwindow* window, int width, int height) {
   
    glfwSetWindowSize (window,  width, height);
    /* Update any perspective matrices used here */

    }
}
