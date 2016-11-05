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

#include <string>

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

class Window{
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
	GLFWwindow* getGLFWWindowPtr();

	// Event Loop Functions	
	virtual void init();
	virtual void update();
	void computeMetrics();
	void render();

	// Event Handling Functions
	virtual void handleCharCallback(unsigned int);
	virtual void handleFramebufferSizeEvent(int, int);
	virtual void handleKeyEvent(int, int, int, int);
	virtual void handleMouseButtonEvent(int, int, int);
	virtual void handleMouseMoveEvent(double, double);
	virtual void handleMouseScrollEvent(double, double);
	virtual void handleWindowSizeEvent(int, int);

	// Callback Functions
protected:

	GLFWwindow* pWindow = nullptr;		//!< GLFW window object; this must be freed upon destruction

	/** An ImGui context specific to this window; avoids clashes in imgui functionality between windows */
	struct ImGuiContext *imguiContext = nullptr;

	std::string title = "Astrohelion Window";	//!< Window title string

	GLfloat frame_dt = 0.0f;			//!< Time interval between the last two frames
	GLfloat lastFrameTime = 0.0f;		//!< Time since app start for the last frame

	int width = 1280;					//!< Window width, pixels
	int height = 720;					//!< Window height, pixels
	int bufferWidth = 1280;				//!< Width of the frame buffer, pixels
	int bufferHeight = 720;				//!< Height of the frame buffer, pixels

	bool bKeyPressed[1024] = {false};	//!< Boolean array; index is the key id
	bool bMouse_firstFrame = true;		//!< Whether or not this is the first frame
	bool bMousePressed[3] = {false};	//!< Boolean array; index is the button id

	GLfloat mouse_lastX = 0;			//!< Mouse x-coordinate from previous frame, pixels
	GLfloat mouse_lastY = 0;			//!< Mouse y-coordinate from previous frame, pixels
	GLfloat mouse_xOffset = 0;			//!< Distance the mouse moved in the x direction between this frame and the previous one, pixels
	GLfloat mouse_yOffset = 0;			//!< Distance the mouse moved in the y direction between this frame and the previous one, pixels
	GLfloat mouse_scrollXOffset = 0;	//!< Mouse x-offset (scroll) from previous frame, pixels
	GLfloat mouse_scrollYOffset = 0;	//!< Mouse y-offset (scroll) from previous frame, pixels


	unsigned int imgui_VBO = 0;		//!< Vertex Buffer Object for ImGui stuff
	unsigned int imgui_VAO = 0;		//!< Vertex Array Object for ImGui stuff
	unsigned int imgui_EBO = 0;		//!< Element Buffer Object for ImGui stuff

	void preDraw();
	virtual void draw();
	void postDraw();

	void ImGui_init();
	void ImGui_createDeviceObjects();
	void ImGui_RenderDrawLists(struct ImDrawData* draw_data);
	void copyMe(const Window&);
};

}// End of gui namespace
}// End of astrohelion namespace