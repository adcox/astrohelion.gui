/**
 *  @brief Translation tutorial
 *  @details Dependencies for this project include:
 *  
 *  * GLFW
 *  * GLEW
 *  * SOIL: https://github.com/smibarber/libSOIL
 *  * glm: https://github.com/g-truc/glm
 *  
 *	Resources consulted to create this file:
 *	
 *	* http://learnopengl.com/#!Getting-started/Creating-a-window
 */

#include <GL/glew.h>		// This header must be included BEFORE glfw
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "App.hpp"
#include "GLErrorHandling.hpp"
#include "Polyline.hpp"
#include "ResourceManager.hpp"

#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

namespace astroGui = astrohelion::gui;

void keyCallback(GLFWwindow*, int, int, int, int);

int main(){
    astroGui::App app;  // Create an App to store global resource manager
    app.init();         // initialize GLFW and GLEW 

    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello World!", nullptr, nullptr);
    if(window == nullptr){
    	std::cout << "Failed to create GLFW window" << std::endl;
    	glfwTerminate();
    	return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;		// This tells GLEW to use more modern techniques for managing OpenGL functionality
    if(glewInit() != GLEW_OK){
    	std::cout << "Failed to initialize GLEW" << std::endl;
    	return EXIT_FAILURE;
    }

    // Get the window width and height, tell OpenGL the size of the viewport; Rather than typing in 800x600, this
    // allows the code to account for high dpi displays (like OS X retina)
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0,0,width,height);

    astroGui::checkForGLErrors("Post-Initialization");

    // std::shared_ptr<astroGui::ResourceManager> resourceManager(new astroGui::ResourceManager());
    std::shared_ptr<astroGui::ResourceManager> resourceManager = app.getResMan();
    astroGui::Shader lineShader = resourceManager->loadShader("../shaders/line_thick.vs",
        "../shaders/line_thick.frag", "../shaders/line_thick.geom", "line_thick");

    std::vector<float> points = {
        -0.5, -0.5, 0,
        -0.5, 0.5, 0,
        0.5, 0.5, 0,
        0.5, -0.5, 0,
        -0.5, -0.5, 0
    };

    astroGui::Polyline line(points);

    astroGui::checkForGLErrors("Post VBO, EBO, VAO Initialization");

    // Create dummy model-view-projection matrix
    glm::mat4 modelViewProj;
    lineShader.setVector2f("viewportSize", width, height, true);

    glClear(GL_COLOR_BUFFER_BIT);   // Clear buffer from any previous usage
    glfwSetKeyCallback(window, keyCallback);

    astroGui::checkForGLErrors("After init shader properties");
    GLfloat angle = 0;
    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();   // Checks if any events are triggered and calls the corresponding functions (via callback methods)

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);   // Set the background color, essentially
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate the shader
        angle = glm::radians(1.0f);
        modelViewProj = glm::rotate(modelViewProj, angle, glm::vec3(0.0f, 0.1f, 0.0f));
        lineShader.setMatrix4("modelViewProjectionMatrix", modelViewProj, true);
        
        line.draw();

        astroGui::checkForGLErrors("Post-Drawing");

        glfwSwapBuffers(window);    // swap the front and back buffers for smooth rendering
    }

    glfwTerminate();		// Clean up and delete all resources that were allocated

	return EXIT_SUCCESS;
}//====================================================

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode){
	(void) scancode;
	(void) mode;
	// When a user presses the ESC key, we set the window should close property to true, closing the application
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}//====================================================