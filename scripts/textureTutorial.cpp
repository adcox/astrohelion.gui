/**
 *  @brief Texture tutorial
 *  @details Dependencies for this project include:
 *  
 *  * GLFW
 *  * GLEW
 *  * SOIL: https://github.com/smibarber/libSOIL
 *
 *	Resources consulted to create this file:
 *	
 *	* http://learnopengl.com/#!Getting-started/Creating-a-window
 */

#include "GL/glew.h"		// This header must be included BEFORE glfw3
#include "GLFW/glfw3.h"
#include "SOIL/SOIL.h"

#include "Shader.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cmath>
#include <iostream>

#define PI 3.1415926

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

    astroGui::Shader ourShader("../shaders/textured.vs", "../shaders/textured.frag");
    // astroGui::Shader ourShader("../shaders/basic.vert", "../shaders/basic.frag");

    GLfloat vertices[] = {
    	// Positions          // Colors           // Texture Coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // Top Right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // Bottom Right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // Bottom Left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // Top Left
    };

    GLuint indices[] = {
        0, 1, 3,    // First triangle
        1, 2, 3     // Second triangle
    };

    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    //Initialization Code (donce once, unless your object frequently changes):
    // 1. Bind the vertex array object
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Copy vertices into buffer
    /*	GL_STATIC_DRAW: data will most likely not change at all or very rarely
     *	GL_DYNAMIC_DRAW: data is likely to change a lot
     *	GL_STREAM_DRAW: the data will change every time it is drawn
     */
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/** Set our vertex attributes pointers
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Similar for color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Similar for texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (GLvoid*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // 4. Unbind the VAO
    glBindVertexArray(0);

    // Load and create a texture
    int texWidth, texHeight;
    unsigned char* image = SOIL_load_image("../textures/container.jpg", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned char* image2 = SOIL_load_image("../textures/tile.jpg", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image2);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Do drawing!
    glClear(GL_COLOR_BUFFER_BIT);	// Clear buffer from any previous usage
    glfwSetKeyCallback(window, keyCallback);
    while(!glfwWindowShouldClose(window)){
    	
        // Form transformation matrix: operations are applied in the order
        // here, i.e., the object will first be translated and then rotated
        // ORDER MATTERS!
        glm::mat4 trans;
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (GLfloat)glfwGetTime() * (GLfloat)PI/4.f, glm::vec3(0.0, 0.0, 1.0));  // 45 deg about z-axis
        // trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));                // 1/2 size in every direction

        glfwPollEvents();	// Checks if any events are triggered and calls the corresponding functions (via callback methods)

    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// Set the background color, essentially
		glClear(GL_COLOR_BUFFER_BIT);

    	// Activate the shader
    	ourShader.use();
    	
        GLuint transformLoc = glGetUniformLocation(ourShader.getID(), "transform");
        // Transform (property location, # matrices, transpose the matrix?, ptr to the matrix)
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        // 5. Draw the object
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(ourShader.getID(), "ourTexture1"), 0); // set the location of the uniform samplers
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.getID(), "ourTexture2"), 1);

        glBindVertexArray(VAO);
    	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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