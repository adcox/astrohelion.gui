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
void doMovement();

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

bool keysPressed[1024] = {false};

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

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

    glEnable(GL_DEPTH_TEST);

    // Get the window width and height, tell OpenGL the size of the viewport; Rather than typing in 800x600, this
    // allows the code to account for high dpi displays (like OS X retina)
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0,0,width,height);

    astroGui::Shader ourShader("../shaders/texture3D.vs", "../shaders/textured.frag");
    // astroGui::Shader ourShader("../shaders/basic.vert", "../shaders/basic.frag");

    // Create transformation matrices
    glm::mat4 view;


    glm::mat4 projection;
    // Perspective projection (FOV, aspect ratio, near clip, far clip)
    projection = glm::perspective(glm::radians(45.f), ((float)width)/((float)height), 0.1f, 100.0f);

    // Create an cube
    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f), 
      glm::vec3( 2.0f,  5.0f, -15.0f), 
      glm::vec3(-1.5f, -2.2f, -2.5f),  
      glm::vec3(-3.8f, -2.0f, -12.3f),  
      glm::vec3( 2.4f, -0.4f, -3.5f),  
      glm::vec3(-1.7f,  3.0f, -7.5f),  
      glm::vec3( 1.3f, -2.0f, -2.5f),  
      glm::vec3( 1.5f,  2.0f, -2.5f), 
      glm::vec3( 1.5f,  0.2f, -1.5f), 
      glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO

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

        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();	// Checks if any events are triggered and calls the corresponding functions (via callback methods)
        doMovement();
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);	// Set the background color, essentially
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    	// Activate the shader
    	ourShader.use();
    	
        GLuint modelLoc = glGetUniformLocation(ourShader.getID(), "model");
        GLuint viewLoc = glGetUniformLocation(ourShader.getID(), "view");
        GLuint projLoc = glGetUniformLocation(ourShader.getID(), "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // 5. Draw the object
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(ourShader.getID(), "ourTexture1"), 0); // set the location of the uniform samplers
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glUniform1i(glGetUniformLocation(ourShader.getID(), "ourTexture2"), 1);

        glBindVertexArray(VAO);
        for(GLuint i = 0; i < 10; i++){
            glm::mat4 model;
            model = glm::translate(model, cubePositions[i]);
            GLfloat angle = glm::radians(20.0f)*i;
            
            if(i%3 == 0)
                angle *= glfwGetTime();

            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
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

    if(action == GLFW_PRESS)
        keysPressed[key] = true;
    else if(action == GLFW_RELEASE)
        keysPressed[key] = false;
}//====================================================

void doMovement(){
    GLfloat cameraSpeed = 5.0f * deltaTime;
    if(keysPressed[GLFW_KEY_W])
        cameraPos += cameraSpeed * cameraFront;
    if(keysPressed[GLFW_KEY_S])
        cameraPos -= cameraSpeed * cameraFront;
    if(keysPressed[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if(keysPressed[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}//====================================================






