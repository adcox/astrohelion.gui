/**
 *  @file App.cpp
 *	@brief 
 *
 *	@author Andrew Cox
 *	@version October 14, 2016
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
#include <iostream>
#include <stdexcept>

#include <GL/glew.h>	// This header must be included BEFORE glfw3
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>

#include "App.hpp"
#include "DemoWindow.hpp"
#include "GLErrorHandling.hpp"
#include "ResourceManager.hpp"
#include "Window.hpp"

namespace astrohelion{
namespace gui{

// Declare the global extern variable
App* GLOBAL_APP = nullptr;


//-----------------------------------------------------
//      *structors
//-----------------------------------------------------

/**
 *  @brief Construct a default App
 */
App::App(){
	GLOBAL_APP = this;
}//====================================================

/**
 *  @brief Copy constructor; create an app that is a replica of <tt>a</tt>
 *  @param a An App reference
 */
App::App(const App& a){
	copyMe(a);
}//====================================================

/**
 *  @brief Destruct the app and free any allocated resources
 */
App::~App(){
	for(auto& window : windows){
		delete window;
	}

	resourceMan->clear();
	glfwTerminate();
}//====================================================

//-----------------------------------------------------
//      Operator Functions
//-----------------------------------------------------

/**
 *  @brief Set this App equal to another
 * 
 *  @param a An App reference
 *  @return A reference to this App
 */
App& App::operator =(const App& a){
	copyMe(a);
	return *this;
}//====================================================

//-----------------------------------------------------
//      Event Loop Functions
//-----------------------------------------------------

/**
 *  @brief Initialize the application
 *  @details This function inits GLFW and creates the resource manager.
 *  It must be called before any windows or other objects can be created
 */
void App::init(){
	glfwSetErrorCallback(GLFWErrorCallback);

	if(glfwInit()){
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// OpenGL major version 3
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// OpenGL minor version 3 -> version 3.3
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	    // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	   	
#if __APPLE__
	    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// Required on OS X
#endif

	}else{
		throw std::runtime_error("Application: Failed to initialize glfw");
	}

	// Create resource manager; wait to load textures and shaders until a window has been created (and GLEW has been initialized)
	resourceMan = std::shared_ptr<ResourceManager>(new ResourceManager());
}//====================================================

/**
 *  @brief Run the application loop
 *  @details This function must be called after all initializations are complete. Note that
 *  A window MUST be created before this function is called to initialized GLEW. This is required
 *  so that the default ImGui shaders and textures can be loaded.
 *  
 *  This function will loop infinitely until the application is told to quit.
 */
void App::run(){
	// A window must be created (to initialize GLEW) before shaders and textures can be compiled
	resourceMan->loadShader("../shaders/imgui.vs", "../shaders/imgui.frag", nullptr, "imgui");
	resourceMan->loadShader("../shaders/line_thick.vs", "../shaders/line_thick.frag",
		"../shaders/line_thick.geom", "line_thick");
	resourceMan->loadShader("../shaders/textured.vs", "../shaders/textured.frag", nullptr, "textured");
	resourceMan->loadShader("../shaders/colored.vs", "../shaders/colored.frag", nullptr, "colored");
	resourceMan->loadShader("../shaders/basic.vert", "../shaders/basic.frag", nullptr, "basic");
	
	// Create a texture for ImGui Font
	Texture2D tempTex;
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int w, h;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &w, &h);   // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.
	tempTex.internalFormat = GL_RGBA;
	tempTex.imageFormat = GL_RGBA;
	tempTex.generate(w, h, pixels);

	resourceMan->addTexture("imguiFont", tempTex);

	// Initialize all windows
	for(auto& window : windows){

		// Set mainWindow if it hasn't been by the user
		if(!mainWindow && windows.size() > 0)
			setMainWindow(window);

		makeContextCurrent(window);
		window->init();

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	while(!shouldClose()){
		glfwPollEvents();

		for(const auto& window : windows){
			makeContextCurrent(window);

			window->computeMetrics();
			window->update();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			window->render();

			glfwSwapBuffers(window->getGLFWWindowPtr());
		}
	}
	
	for(auto& window : windows){
		delete window;
	}

}//====================================================


//-----------------------------------------------------
//      Set and Get Fucntions
//-----------------------------------------------------

/**
 *  @brief Retrieve a pointer to the main application window.
 *  @return a pointer to the main application window.
 */
Window* App::getMainWindow() const{ return mainWindow; }

/**
 *  @brief Retrieve a shared pointer to the application resource manager
 *  @return a shared pointer to the application resource manager
 */
std::shared_ptr<ResourceManager> App::getResMan() const { return resourceMan; }

/**
 *  @brief Set the main window. If this window is closed, the application will quit
 *  @param pWin A pointer to the window object
 */
void App::setMainWindow(Window* pWin){ mainWindow = pWin; }


//-----------------------------------------------------
//      Utility Functions
//-----------------------------------------------------

/**
 *  @brief Add a window to the application
 *  @details The window should be allocated to the stack via <tt>new</tt>. The
 *  App object will delete the window once it is closed. Take care to avoid
 *  dereferencing a window pointer once the window has been closed
 * 
 *  @param title Title string that will display on the window decoration
 *  @param pMonitor Pointer to the monitor object to create the window on; set to nullptr to skip this option
 *  @param share Pointer to a window to share resources with; This should be used in general to avoid duplicating
 *  shaders, textures, and other resources. Set <tt>share</tt> to the main window pointer for all secondary 
 *  windows
 *  @return True if window creation is successful, false otherwise
 */
bool App::addWindow(Window* pWindow, const char* title, GLFWmonitor *pMonitor, Window* share){
	Window* prevContext = currentWindow;

	try{
    	pWindow->create(title, pMonitor, share);
    }catch(std::runtime_error &e){
    	delete pWindow;
    	return false;
    }

	makeContextCurrent(pWindow);    
	
	// Save the user pointer so we can retrieve the "owner" of the GLFW window
	glfwSetWindowUserPointer(pWindow->getGLFWWindowPtr(), pWindow);

    // Set any OpenGL options
    glEnable(GL_DEPTH_TEST);

    checkForGLErrors("App::addWindow() OpenGL Options Error");

    // add new window to the list
    windows.push_back(pWindow);

    // Restore previous context
    makeContextCurrent(prevContext);
    return true;
}//====================================================

/**
 *  @brief Set the parameters of this app to be identical to those of the
 *  input app reference
 * 
 *  @param a An App reference
 */
void App::copyMe(const App& a){
	mainWindow = a.mainWindow;
	currentWindow = a.currentWindow;
	resourceMan = a.resourceMan;
	windows = a.windows;
}//====================================================

/**
 *  @brief Set the window to be the current, focussed window
 *  @param pWin Pointer to the window to make the current context
 */
void App::makeContextCurrent(Window* pWin){
	if(pWin){
		glfwMakeContextCurrent(pWin->getGLFWWindowPtr());
		currentWindow = pWin;
	}else{
		std::cout << "makeContextCurrent: Invalid window pointer!" << std::endl;
	}
}//====================================================

/**
 *  @brief Determine whether or not the application should quit.
 *  @details This function should be called once per frame. Windows that have been closed 
 *  will be deleted and removed from the relevant lists. If all windows are closed, or the main
 *  window is closed, a value of TRUE is returned.
 *  @return whether or not the application should quit.
 */
bool App::shouldClose(){
	if(windows.empty())
		return true;

	bool mainWindowClosed = false;

	std::list<Window*> winToDelete;
	for(const auto& win : windows){
		if(glfwWindowShouldClose(win->getGLFWWindowPtr())){
			winToDelete.push_back(win);

			if(GLOBAL_APP->getMainWindow() == win)
				mainWindowClosed = true;
		}
	}

	if(mainWindowClosed)
		winToDelete = windows;	// Delete all the windows if the main window is closed

	if(!winToDelete.empty()){
		for(auto& win : winToDelete){			
			delete win;
			windows.remove(win);
		}
	}

	return windows.empty();
}//====================================================





}	// END of gui namespace
}	// END of astrohelion namespace