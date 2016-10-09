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

#pragma once

#include "GL/glew.h"	// This header must be included BEFORE glfw3
#include "GLFW/glfw3.h"

#include "ResourceUser.hpp"

namespace astrohelion{
namespace gui{

// Forward Declarations

/**
 * 	@brief A default window
 * 	@details More useful information about GLFW windows can be found at
 * 	http://www.glfw.org/docs/3.0/window.html
 * 	
 * 	@author Andrew Cox
 * 	@version Sep 19, 2016
 * 	@copyright GNU GPL v3.0
 */

class Window : public ResourceUser{
public:
	// *structors
	Window();
	Window(int, int);
	Window(int, int, const char* title, GLFWmonitor *pMonitor = nullptr, Window* share = nullptr);
	virtual ~Window();
	Window(const Window&);

	void create(const char*, GLFWmonitor*, Window*);

	// Operators
	Window& operator =(const Window&);

	// Set and Get Functions
	unsigned int getID() const;
	GLFWwindow* getWindowPtr();

	void setID(unsigned int);

	// Event Loop Functions	
	virtual void init();
	virtual void update();
	void computeMetrics();
	virtual void draw();

	// Event Handling Functions
	virtual void handleKeyEvent(int, int, int, int);
	virtual void handleMouseMoveEvent(double, double);
	virtual void handleMouseScrollEvent(double, double);

	// Callback Functions
protected:

	GLFWwindow* pWindow = nullptr;
	unsigned int id = 0;
	std::string title = "Astrohelion Window";

	GLfloat frame_dt = 0.0f;		//!< Time interval between the last two frames
	GLfloat lastFrameTime = 0.0f;	//!< Time since app start for the last frame

	int width = 1280;
	int height = 720;
	int bufferWidth = 1280;
	int bufferHeight = 720;

	bool bKeyPressed[1024] = {false};
	bool bMouse_firstFrame = true;

	GLfloat mouse_lastX = 0;
	GLfloat mouse_lastY = 0;
	GLfloat mouse_xOffset = 0;
	GLfloat mouse_yOffset = 0;
	GLfloat mouse_scrollXOffset = 0;
	GLfloat mouse_scrollYOffset = 0;

	void copyMe(const Window&);

private:

};

}// End of gui namespace
}// End of astrohelion namespace