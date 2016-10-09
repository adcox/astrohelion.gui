
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Shader.hpp"
#include "ResourceManager.hpp"

#include <cmath>
#include <iostream>
#include <list>
#include <memory>

namespace astroGui = astrohelion::gui;

struct Window{
	int width = 0;
	int height = 0;

	unsigned int id = 0;
	GLFWwindow* pHandle = nullptr;
	unsigned int VAO = 0;
};//====================================================

unsigned int windowCount = 0;

std::list<Window*> windows;
Window* currentContext = nullptr;

Window* createWindow(int width, int height, const char* title, GLFWmonitor* pMonitor = nullptr, Window* share = nullptr);
void makeContextCurrent(Window* pWin);
bool shouldClose();

void GLFWErrorCallback(int err, const char* desc);
void checkForGLErrors(const char* msg);

int main(){
	glfwSetErrorCallback(GLFWErrorCallback);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// OpenGL major version 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// OpenGL minor version 3 -> version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// Required on OS X

    Window* win1 = createWindow(800, 600, "Window 1");
    createWindow(800, 400, "Window 2", nullptr, win1);

    makeContextCurrent(win1);

    GLfloat vertices[] = {
        // Positions            // Colors
    	-0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   // Bottom left
    	0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   // Bottom right
    	0.0f, 0.5f, 0.0f,      0.0f, 0.0f, 1.0f    // Top
    };

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy vertices into buffer
    /*	GL_STATIC_DRAW: data will most likely not change at all or very rarely
     *	GL_DYNAMIC_DRAW: data is likely to change a lot
     *	GL_STREAM_DRAW: the data will change every time it is drawn
     */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    std::shared_ptr<astroGui::ResourceManager> resourceManager(new astroGui::ResourceManager());
	astroGui::Shader ourShader = resourceManager->loadShader("../shaders/basic.vert", "../shaders/basic.frag", nullptr, "basic");

	for(auto& win : windows){
		makeContextCurrent(win);

		// Setup VAO (not shared between contexts)
		glGenVertexArrays(1, &(win->VAO));
		glBindVertexArray(win->VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Set vertex attributes
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Set color attributes
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

        checkForGLErrors("VAO Creation Error");

        glBindVertexArray(0);
	}
    
    for(const auto& win : windows){
    	makeContextCurrent(win);
    	glClear(GL_COLOR_BUFFER_BIT);
    }

    while(!shouldClose()){

    	for(const auto& win : windows){
    		makeContextCurrent(win);

    		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// Set the background color, essentially
			glClear(GL_COLOR_BUFFER_BIT);

			ourShader.use();

			glBindVertexArray(win->VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			glBindVertexArray(0);
			glfwSwapBuffers(win->pHandle);

			checkForGLErrors("Render Error");
    	}

    	glfwPollEvents();
    }

    for(auto& win : windows){
    	glfwDestroyWindow(win->pHandle);
    	delete win;
    }

    glfwTerminate();		// Clean up and delete all resources that were allocated

    return EXIT_SUCCESS;
}//====================================================

Window* createWindow(int width, int height, const char* title, GLFWmonitor* pMonitor, Window* share){
    Window* prevContext = currentContext;

    Window* window = new Window();
    window->width = width;
    window->height = height;
    window->id = windowCount++;

    if(share){
    	window->pHandle = glfwCreateWindow(width, height, title, pMonitor, share->pHandle);
    }else{
    	window->pHandle = glfwCreateWindow(width, height, title, pMonitor, nullptr);
    }
    
    if(window->pHandle == nullptr){
    	std::cout << "Failed to create GLFW window 2" << std::endl;
    	delete window;
    	return nullptr;
    }

    makeContextCurrent(window);

    // Init GLEW for this context
    glewExperimental = GL_TRUE;		// This tells GLEW to use more modern techniques for managing OpenGL functionality
    if(glewInit() != GLEW_OK){
    	std::cout << "Failed to initialize GLEW" << std::endl;
    	glfwDestroyWindow(window->pHandle);
    	delete window;
    	return nullptr;
    }

    // Set OpenGL Options
    // Get the window width and height, tell OpenGL the size of the viewport; Rather than typing in 800x600, this
    // allows the code to account for high dpi displays (like OS X retina)
    int w, h;
    glfwGetFramebufferSize(window->pHandle, &w, &h);
    glViewport(0,0,w,h);
    // glEnable(GL_DEPTH_TEST);
    // glEnable(GL_CULL_FACE);

    checkForGLErrors("OpenGL Options Error");
    // Setup callbacks

    // add new window to the list
    windows.push_back(window);

    // Restore previous context
    makeContextCurrent(prevContext);

    return window;
}//====================================================

void makeContextCurrent(Window* pWin){
	if(pWin){
		glfwMakeContextCurrent(pWin->pHandle);
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
		if(glfwWindowShouldClose(win->pHandle)){
			winToDelete.push_back(win);
		}
	}

	if(!winToDelete.empty()){
		for(auto& win : winToDelete){
			glfwDestroyWindow(win->pHandle);
			delete win;
			windows.remove(win);
		}
	}

	return windows.empty();
}//====================================================

