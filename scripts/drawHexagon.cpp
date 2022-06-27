/**
 *  @brief First tutorial
 *  @details Dependencies for this project include:
 *  
 *  * GLFW
 *  * GLEW
 *  * SOIL
 *
 *	Resources consulted to create this file:
 *	
 *	* http://learnopengl.com/#!Getting-started/Creating-a-window
 */

#include "GL/glew.h"		// This header must be included BEFORE glfw3
#include "GLFW/glfw3.h"

#include "ResourceManager.hpp"
#include "Shader.hpp"

#include <cmath>
#include <iostream>
#include <memory>

namespace astroGui = astrohelion::gui;

void keyCallback(GLFWwindow*, int, int, int, int);

int main(){

	std::cout << "Hello world!" << std::endl;

	glfwInit();	
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// OpenGL major version 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					// OpenGL minor version 3 -> version 3.3
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// Required on OS X

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

    GLfloat vertices[] = {
         1.00,  0.00,  0.00,  0.90,  0.90,  0.90,
         0.50,  0.87,  0.00,  0.90,  0.90,  0.90,
        -0.50,  0.87,  0.00,  0.90,  0.90,  0.90,
        -1.00, -0.00,  0.00,  0.90,  0.90,  0.90,
        -0.50, -0.87,  0.00,  0.90,  0.90,  0.90,
         0.50, -0.87,  0.00,  0.90,  0.90,  0.90
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5
    };

    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glGenBuffers(1, &EBO);

    // Copy vertices into buffer
    /*	GL_STATIC_DRAW: data will most likely not change at all or very rarely
     *	GL_DYNAMIC_DRAW: data is likely to change a lot
     *	GL_STREAM_DRAW: the data will change every time it is drawn
     */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    std::shared_ptr<astroGui::ResourceManager> resourceManager(new astroGui::ResourceManager());
    astroGui::Shader ourShader = resourceManager->loadShader("../shaders/basic.vert", "../shaders/basic.frag", nullptr, "basic");

    GLuint VAO;
    glGenVertexArrays(1, &VAO);

    //Initialization Code (donce once, unless your object frequently changes):
    // 1. Bind the vertex array object
    glBindVertexArray(VAO);
    	// 2. Copy our vertices in a buffer for OpenGL to use
    	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    	
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // 3. Set our vertex attributes pointers
    	/**
	     *  @brief [brief description]
	     *  @details [long description]
	     * 
	     *  @param 1 specifies which vertex attribute to configure; shader object specified location = 0, so we use 0
	     *  @param 2 size of the vertex attribute: vec3 has 3 values
	     *  @param 3 type of data: float
	     *  @param 4 Whether we want data to be normalized: false
	     *  @param 5 stride, or space between consecutive vertex attribute sets; tightly packed, so each set is 6 floats long: 3 position, 3 color
	     *  @param 6 offset of where the position data begins in the buffer; position data is at start of set, so value is 0
	     */
    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // Similar for color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GLfloat)));
        glEnableVertexAttribArray(1);

    // 4. Unbind the VAO
    glBindVertexArray(0);

    glClear(GL_COLOR_BUFFER_BIT);	// Clear buffer from any previous usage
    glfwSetKeyCallback(window, keyCallback);
    while(!glfwWindowShouldClose(window)){
    	glfwPollEvents();	// Checks if any events are triggered and calls the corresponding functions (via callback methods)

    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// Set the background color, essentially
		glClear(GL_COLOR_BUFFER_BIT);

    	// Activate the shader
    	ourShader.use();
    	
        // 5. Draw the object
        glBindVertexArray(VAO);
    	// glDrawArrays(GL_TRIANGLES, 0, 6);
        // glDrawArrays(GL_LINE_LOOP, 0, 3);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
    	glBindVertexArray(0);
    	glfwSwapBuffers(window);	// swap the front and back buffers for smooth rendering
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