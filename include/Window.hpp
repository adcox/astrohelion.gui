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

#include "imgui/imgui.h"

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
	Window(int, int, int, const char* title, GLFWmonitor *pMonitor = nullptr, Window* share = nullptr);
	virtual ~Window();
	Window(const Window&);

	void create(const char*, GLFWmonitor*, Window*);
	void destruct();

	// Operators
	Window& operator =(const Window&);

	// Set and Get Functions
	const char* getClipboardText() const;
	int getID() const;
	GLFWwindow* getWindowPtr();

	void setClipboardText(const char*);
	void setID(int);

	// Event Loop Functions	
	virtual void init();
	virtual void update();
	void computeMetrics();
	virtual void preDraw();
	virtual void draw();
	virtual void postDraw();

	// Event Handling Functions
	virtual void handleCharCallback(unsigned int);
	virtual void handleKeyEvent(int, int, int, int);
	virtual void handleMouseButtonEvent(int, int, int);
	virtual void handleMouseMoveEvent(double, double);
	virtual void handleMouseScrollEvent(double, double);

	// Callback Functions
protected:

	GLFWwindow* pWindow = nullptr;
	int id = -1;

	void *imguiContext = nullptr;

	std::string title = "Astrohelion Window";

	GLfloat frame_dt = 0.0f;		//!< Time interval between the last two frames
	GLfloat lastFrameTime = 0.0f;	//!< Time since app start for the last frame

	int width = 1280;
	int height = 720;
	int bufferWidth = 1280;
	int bufferHeight = 720;

	bool bKeyPressed[1024] = {false};
	bool bMouse_firstFrame = true;
	bool bMousePressed[3] = {false};

	GLfloat mouse_lastX = 0;
	GLfloat mouse_lastY = 0;
	GLfloat mouse_xOffset = 0;
	GLfloat mouse_yOffset = 0;
	GLfloat mouse_scrollXOffset = 0;
	GLfloat mouse_scrollYOffset = 0;

	GLuint imgui_fontTexture = 0;

	unsigned int imgui_VBO = 0;		//!< Vertex Buffer Object for ImGui stuff
	unsigned int imgui_VAO = 0;		//!< Vertex Array Object for ImGui stuff
	unsigned int imgui_EBO = 0;		//!< Element Buffer Object for ImGui stuff

	// static int          g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;

	bool createFontsTexture();
	bool ImGui_init();
	bool ImGui_createDeviceObjects();
	void ImGui_RenderDrawLists(ImDrawData* draw_data);
	void copyMe(const Window&);

private:

};

}// End of gui namespace
}// End of astrohelion namespace