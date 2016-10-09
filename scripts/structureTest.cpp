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
#include <memory>
#include <stdexcept>

#include "DemoWindow.hpp"
#include "ResourceManager.hpp"

#include "GL/glew.h"	// This header must be included BEFORE glfw3
#include "GLFW/glfw3.h"

#include "imgui/imgui_impl_glfw_gl3.h"

namespace astroGui = astrohelion::gui;

astroGui::DemoWindow window;

void keyEventCallback(GLFWwindow*, int, int, int, int);
void mouseMoveCallback(GLFWwindow*, double, double);
void mouseScrollCallback(GLFWwindow*, double, double);
void mouseButtonCallback(GLFWwindow*, int, int, int);
void charCallback(GLFWwindow*, unsigned int);

int main(){
	if(glfwInit()){
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// OpenGL major version 3
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// OpenGL minor version 3 -> version 3.3
	    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	    // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	   	
#if __APPLE__
	    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// Required on OS X
#endif

	    // Make a window
	    // std::shared_ptr<Window> win = std::make_shared<DemoWindow>();
    	// windows.push_back(win);
	}else{
		throw std::runtime_error("Application: Failed to initialize glfw");
	}

	window.create();

	std::shared_ptr<astroGui::ResourceManager> resourceManager(new astroGui::ResourceManager());
	resourceManager->loadShader("../shaders/texture3D.vs", "../shaders/textured.frag", nullptr, "cube");
	resourceManager->loadTexture("../textures/container.jpg", false, "container");

	window.setResourceManager(resourceManager);

	if(window.getWindowPtr()){
		// Set any callback functions
    	glfwSetKeyCallback(window.getWindowPtr(), keyEventCallback);
    	glfwSetCursorPosCallback(window.getWindowPtr(), mouseMoveCallback);
    	glfwSetScrollCallback(window.getWindowPtr(), mouseScrollCallback);
    	glfwSetMouseButtonCallback(window.getWindowPtr(), mouseButtonCallback);
    	glfwSetCharCallback(window.getWindowPtr(), charCallback);

		window.init();

		while(!glfwWindowShouldClose(window.getWindowPtr())){
			glfwPollEvents();
			window.computeMetrics();

			window.update();

			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			window.draw();

			glfwSwapBuffers(window.getWindowPtr());	
		}
	}
	
	resourceManager->clear();
	glfwTerminate();

	return EXIT_SUCCESS;
}//====================================================

void keyEventCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods){
	// TODO - For multiple windows, will need to detect window from the pointer -> use a map?
	window.handleKeyEvent(key, scancode, action, mods);
	ImGui_ImplGlfwGL3_KeyCallback(pWindow, key, scancode, action, mods);
}//====================================================

void mouseMoveCallback(GLFWwindow* pWindow, double xpos, double ypos){
	// TODO - For multiple windows, will need to detect window from the pointer -> use a map?
	window.handleMouseMoveEvent(xpos, ypos);
}//====================================================

void mouseScrollCallback(GLFWwindow* pWindow, double xoffset, double yoffset){
	// TODO - For multiple windows, will need to detect window from the pointer -> use a map?
	window.handleMouseScrollEvent(xoffset, yoffset);
	ImGui_ImplGlfwGL3_ScrollCallback(pWindow, xoffset, yoffset);
}//====================================================

void mouseButtonCallback(GLFWwindow* pWindow, int button, int action, int mods){
	ImGui_ImplGlfwGL3_MouseButtonCallback(pWindow, button, action, mods);
}//====================================================

void charCallback(GLFWwindow* pWindow, unsigned int c){
	ImGui_ImplGlfwGL3_CharCallback(pWindow, c);
}//====================================================