/**
 *  @brief First tutorial
 *  @details Dependencies for this project include:
 *  
 *  * GLFW
 *  * GLEW
 *  * SOIL - https://github.com/smibarber/libSOIL
 *  * ImGui
 *
 *  Resources consulted to create this file:
 *  
 *  * http://learnopengl.com/#!Getting-started/Creating-a-window
 */
#include <iostream>
#include <list>
#include <memory>
#include <stdexcept>

#include "GL/glew.h"	// This header must be included BEFORE glfw3
#include "GLFW/glfw3.h"

#include "Window.hpp"
#include "DemoWindow.hpp"

#include "GLErrorHandling.hpp"
#include "ResourceManager.hpp"

#include <imgui/imgui.h>
#include "imgui/imgui_impl_glfw_gl3.h"

namespace astroGui = astrohelion::gui;
using namespace astroGui;

/** Contains pointers to all window objects; these must be freed before exiting! */
std::list<Window*> windows;

/** Maps GLFWwindow pointers to their parent Window (custom implementation) objects */
std::map<GLFWwindow*, Window*> glfwWindowMap;

/** Current active window */
Window* currentContext = nullptr;

// Event Loop Functions
bool shouldClose();

// Action / Utility Functions
Window* createWindow(int width, int height, const char* title, GLFWmonitor* pMonitor = nullptr, Window* share = nullptr);
void makeContextCurrent(Window*);

// Event Functions
void keyEventCallback(GLFWwindow*, int, int, int, int);
void mouseMoveCallback(GLFWwindow*, double, double);
void mouseScrollCallback(GLFWwindow*, double, double);
void mouseButtonCallback(GLFWwindow*, int, int, int);
void charCallback(GLFWwindow*, unsigned int);

int main(){
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

	// Create Windows
	Window* win1 = createWindow(800, 600, "Window 1");
	createWindow(1280, 700, "Window 2", nullptr, win1);

	// Create resource objects
	std::shared_ptr<ResourceManager> resourceManager(new ResourceManager());
	resourceManager->loadShader("../shaders/texture3D.vs", "../shaders/textured.frag", nullptr, "cube");
	resourceManager->loadTexture("../textures/container.jpg", false, "container");

	// Initialize all windows
	for(auto& window : windows){
		makeContextCurrent(window);
		window->setResourceManager(resourceManager);
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

			window->draw();

			glfwSwapBuffers(window->getWindowPtr());
		}
	}
	
	for(auto& window : windows){
		glfwDestroyWindow(window->getWindowPtr());
		delete window;
	}

	resourceManager->clear();
	glfwTerminate();

	return EXIT_SUCCESS;
}//====================================================

Window* createWindow(int width, int height, const char* title, GLFWmonitor* pMonitor, Window* share){
    Window* prevContext = currentContext;

    Window* window = new DemoWindow(width, height);

    try{
    	window->create(title, pMonitor, share);
    }catch(std::runtime_error &e){
    	// If the GLFW window was created but other errors failed, delete the GLFW window
    	if(window->getWindowPtr())
    		glfwDestroyWindow(window->getWindowPtr());

    	delete window;
    	return nullptr;
    }

	makeContextCurrent(window);    

    // Set any OpenGL options
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    checkForGLErrors("OpenGL Options Error");
    
    // Setup callbacks
    glfwSetKeyCallback(window->getWindowPtr(), keyEventCallback);
	glfwSetCursorPosCallback(window->getWindowPtr(), mouseMoveCallback);
	glfwSetScrollCallback(window->getWindowPtr(), mouseScrollCallback);
	glfwSetMouseButtonCallback(window->getWindowPtr(), mouseButtonCallback);
	glfwSetCharCallback(window->getWindowPtr(), charCallback);

    // add new window to the list
    windows.push_back(window);
    glfwWindowMap[window->getWindowPtr()] = window;

    // Restore previous context
    makeContextCurrent(prevContext);

    return window;
}//====================================================

void makeContextCurrent(Window* pWin){
	if(pWin){
		glfwMakeContextCurrent(pWin->getWindowPtr());
		currentContext = pWin;
	}else{
		std::cout << "makeContextCurrent: Invalid window pointer!" << std::endl;
	}
}//====================================================

bool shouldClose(){
	if(windows.empty())
		return true;

	std::list<Window*> winToDelete;
	for(const auto& win : windows){
		if(glfwWindowShouldClose(win->getWindowPtr())){
			winToDelete.push_back(win);
		}
	}

	if(!winToDelete.empty()){
		for(auto& win : winToDelete){
			glfwWindowMap.erase(win->getWindowPtr());
			glfwDestroyWindow(win->getWindowPtr());
			delete win;
			windows.remove(win);
		}
	}

	return windows.empty();
}//====================================================

void keyEventCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods){
	glfwWindowMap[pWindow]->handleKeyEvent(key, scancode, action, mods);
	ImGui_ImplGlfwGL3_KeyCallback(pWindow, key, scancode, action, mods);
}//====================================================

void mouseMoveCallback(GLFWwindow* pWindow, double xpos, double ypos){
	glfwWindowMap[pWindow]->handleMouseMoveEvent(xpos, ypos);
}//====================================================

void mouseScrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset){
	glfwWindowMap[pWindow]->handleMouseScrollEvent(xoffset, yoffset);
	ImGui_ImplGlfwGL3_ScrollCallback(pWindow, xoffset, yoffset);
}//====================================================

void mouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods){
	ImGui_ImplGlfwGL3_MouseButtonCallback(pWindow, button, action, mods);
}//====================================================

void charCallback(GLFWwindow* pWindow, unsigned int c){
	ImGui_ImplGlfwGL3_CharCallback(pWindow, c);
}//====================================================