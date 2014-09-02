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
#include <assert.h>
#include "init.h"
#include "gllog.h"
#include "Scene.h"


namespace gland{
GLFWwindow* init(){     
    
    std::cout <<  "Starting GLFW: " <<  glfwGetVersionString () << std::endl;

    //Check that log works
    assert (gllog::gl_log (message, __FILE__, __LINE__));

    //Set error callback function for logging
    glfwSetErrorCallback (gllog::glfw_error_callback);
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit ()) {
        std::cerr << "ERROR: could not start GLFW3 " << std::endl;
        return nullptr;
    } 

    //Init log file
    gllog::restart_gl_log();

    // uncomment these lines if on Apple OS X
    /*glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
      glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
      glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);
      glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
  
    //anti-aliasing
    glfwWindowHint (GLFW_SAMPLES, 4);

    //vsync
    glfwSwapInterval(1);

    GLFWmonitor* mon = glfwGetPrimaryMonitor ();
    if (!mon) {
        std::cerr << "ERROR: could not obtain primary monitor with GLFW3 "<< std::endl;
        glfwTerminate();
        return nullptr;;
    }
    const GLFWvidmode* vmode = glfwGetVideoMode (mon);
    if (!vmode) {
        std::cerr << "ERROR: could not obtain video mode with GLFW3 "<< std::endl;
        glfwTerminate();
        return nullptr;;
    }

    bool fullscreen = true;

    std::cout << mon << std::endl;
    std::cout << vmode->width << ", " <<  vmode->height << std::endl;

    std::string title = "OpenGL 4 test";

    //****************************************
    GLFWwindow* window;
    if(fullscreen){
	//Fullscreen mode
      window = glfwCreateWindow (vmode->width, vmode->height, title.c_str(), mon, NULL);  
    } else {	
        //Normal window
      window = glfwCreateWindow (vmode->width, vmode->height, title.c_str(), NULL,NULL);
    }
    //****************************************

    if (!window) {
        std::cerr << "ERROR: could not open window with GLFW3 "<< std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwSetInputMode (window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwMakeContextCurrent (window);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    GLenum glewError = glewInit();

    if (glewError != GLEW_OK){
        glfwTerminate();
        return nullptr;
    }

    //Log GL parameters
    gllog::log_gl_params();

    // get version info
    const GLubyte* renderer = glGetString (GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString (GL_VERSION); // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported: " << version << std::endl;
    std::cout << "GLSL version supported: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
 
    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable (GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"

    int width;
    int height;
    glfwGetWindowSize (window, &width, &height);
    glViewport (0, 0, width, height);

    return window;
}
}
