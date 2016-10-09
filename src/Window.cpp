/**
 *  @file 
 *	@brief 
 *
 *	@author Andrew Cox
 *	@version 
 *	@copyright GNU GPL v3.0
 */
 
/*
 *	Astrohelion 
 *	Copyright 2016, Andrew Cox; Protected under the GNU GPL v3.0
 *	
 *	This file is part of Astrohelion
 *
 *  Astrohelion is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Astrohelion is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Astrohelion.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Window.hpp"

#include <stdexcept>

#include <iostream>

namespace astrohelion{
namespace gui{
//-----------------------------------------------------
//      *structors
//-----------------------------------------------------

Window::Window(){

}//====================================================

/**
 *  @brief Construct a window object
 *  @details This function does not create the GLFW window; see Window::create
 *  for that functionality
 * 
 *  @param w window width, pixels
 *  @param h window height, pixesl
 */
Window::Window(int w, int h){
	width = w;
	height = h;
}//====================================================

/**
 *  @brief Construct a window object
 *  @details This function creates a GLFW window. Note that this function
 *  tells GLFW to set this window as the current context.
 * 
 *  @param w window width, pixels
 *  @param h window height, pixesl
 *  @param title window title
 *  @param pMonitor Monitor pointer - see GLFW Window docs for more infor
 *  @param share another window to share resources with (optional)
 */
Window::Window(int w, int h, const char* title, GLFWmonitor *pMonitor, Window* share){
	width = w;
	height = h;
	this->title = title;

	create(title, pMonitor, share);
}//====================================================

Window::~Window(){}

Window::Window(const Window &w){
	copyMe(w);
}//====================================================

/**
 *  @brief Create and initialize a GLFW window.
 *  @details Note that this function tells GLFW to set this window as 
 *  the current context.
 * 
 *  @param w window width, pixels
 *  @param h window height, pixesl
 *  @param title window title
 *  @param pMonitor Monitor pointer - see GLFW Window docs for more infor
 *  @param share another window to share resources with (optional)
 */
void Window::create(const char* title, GLFWmonitor* pMonitor, Window* share){
	this->title = title;
	
	if(share){
		pWindow = glfwCreateWindow(width, height, title, pMonitor, share->getWindowPtr());
	}else{
		pWindow = glfwCreateWindow(width, height, title, pMonitor, nullptr);
	}

	if(pWindow == nullptr){
		throw std::runtime_error("Window::Window: Failed to create GLFW window");
	}

	glfwMakeContextCurrent(pWindow);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		throw std::runtime_error("Window::Window: Failed to initialize GLEW");
	}

	glfwGetFramebufferSize(pWindow, &bufferWidth, &bufferHeight);
    glViewport(0, 0, bufferWidth, bufferHeight);	// make the buffer take up the entire screen
}//====================================================

//-----------------------------------------------------
//      Operators
//-----------------------------------------------------

Window& Window::operator =(const Window &w){
	copyMe(w);
	return *this;
}//====================================================

//-----------------------------------------------------
//      Event Loop Functions
//-----------------------------------------------------

void Window::init(){}

void Window::computeMetrics(){
	GLfloat currentFrame = glfwGetTime();
	frame_dt = currentFrame - lastFrameTime;
	lastFrameTime = currentFrame;

}//====================================================

void Window::update(){}

void Window::draw(){}

//-----------------------------------------------------
//      Event Handling Functions
//-----------------------------------------------------

void Window::handleKeyEvent(int key, int scancode, int action, int mode){
	(void)scancode;
	(void)mode;

	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(pWindow, GL_TRUE);

	if(key >= 0 && key < 1024){
		if(action == GLFW_PRESS){
	        bKeyPressed[key] = true;
		}else if(action == GLFW_RELEASE){
	        bKeyPressed[key] = false;
	    }
	}

	std::cout << title << ": Key " << key << " action!" << std::endl;
}//====================================================

void Window::handleMouseMoveEvent(double xpos, double ypos){
	if(bMouse_firstFrame){
		mouse_lastX = xpos;
		mouse_lastY = ypos;
		bMouse_firstFrame = false;
	}

	mouse_xOffset = xpos - mouse_lastX;
	mouse_yOffset = mouse_lastY - ypos;	// Reversed since y-coordinates go from bottom to top

	mouse_lastX = xpos;
	mouse_lastY = ypos;

	std::cout << title << ": Mouse moved!" << std::endl;
}//====================================================

void Window::handleMouseScrollEvent(double xoffset, double yoffset){
	mouse_scrollXOffset = xoffset;
	mouse_scrollYOffset = yoffset;

	std::cout << title << ": Mouse scrolled!" << std::endl;
}//====================================================

//-----------------------------------------------------
//      Set and Get Functions
//-----------------------------------------------------

unsigned int Window::getID() const { return id; }

GLFWwindow* Window::getWindowPtr(){ return pWindow; }

void Window::setID(unsigned int id){ this->id = id; }

//-----------------------------------------------------
//      Utility Functions
//-----------------------------------------------------

void Window::copyMe(const Window &w){
	ResourceUser::copyMe(w);
	pWindow = w.pWindow;
	id = w.id;
	title = w.title;

	frame_dt = w.frame_dt;
	lastFrameTime = w.lastFrameTime;

	width = w.width;
	height = w.height;
	bufferWidth = w.bufferWidth;
	bufferHeight = w.bufferHeight;

	std::copy(w.bKeyPressed, w.bKeyPressed+1024, bKeyPressed);
	bMouse_firstFrame = w.bMouse_firstFrame;

	mouse_lastX = w.mouse_lastX;
	mouse_lastY = w.mouse_lastY;
	mouse_xOffset = w.mouse_xOffset;
	mouse_yOffset = w.mouse_yOffset;
	mouse_scrollXOffset = w.mouse_scrollXOffset;
	mouse_scrollYOffset = w.mouse_scrollYOffset;

}//====================================================

}// End of gui namespace
}// End of Astrohelion namespace