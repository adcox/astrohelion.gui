/**
 *  @file Demo Window
 *	@brief A demo window with some objects to test out functionaltiy
 *
 *	@author Andrew Cox
 *	@version September 24, 2016
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

#include "DemoWindow.hpp"

#include <cstdio>

namespace astrohelion{
namespace gui{

//-----------------------------------------------------
//      *structors
//-----------------------------------------------------
DemoWindow::DemoWindow(){}
DemoWindow::DemoWindow(int w, int h) : Window(w, h) {}
DemoWindow::DemoWindow(int id, int w, int h, const char* title, GLFWmonitor *pMonitor, Window* share) : Window(id, w, h, title, pMonitor, share) {}

DemoWindow::~DemoWindow(){
	if(pWindow){
		glDeleteVertexArrays(1, &VAO);
    	glDeleteBuffers(1, &VBO);
	}
}//====================================================

void DemoWindow::init(){
    Window::init();

    if(!appRM){
        throw std::runtime_error("DemoWindow::init: Resource Manager has not been loaded; cannot init window");
    }

    // Create a vertex array and a buffer to store the cube data
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    // Give the vertex data to the buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // TexCoord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0); // Unbind VAO

    camera = CameraFPS(glm::vec3(0.0f, 0.0f, 3.f));
}//====================================================

void DemoWindow::update(){
    Window::update();

	if(bKeyPressed[GLFW_KEY_W])
		camera.processKeyboard(CamMove_tp::FORWARD, frame_dt);
	if(bKeyPressed[GLFW_KEY_S])
		camera.processKeyboard(CamMove_tp::BACKWARD, frame_dt);
	if(bKeyPressed[GLFW_KEY_A])
		camera.processKeyboard(CamMove_tp::LEFT, frame_dt);
	if(bKeyPressed[GLFW_KEY_D])
		camera.processKeyboard(CamMove_tp::RIGHT, frame_dt);

	camera.processMouseScroll(mouse_scrollYOffset);

	glm::mat4 view = camera.getViewMatrix();

    glm::mat4 projection;
    projection = glm::perspective(camera.getZoom(), (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);
    
    appRM->getShader("cube").setMatrix4("view", view);
    appRM->getShader("cube").setMatrix4("projection", projection);
}//====================================================

void DemoWindow::draw(){
    Window::draw();
    
    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    {
        ImGui::Text("Hello, world!");
        ImGui::SliderFloat("float", &imgui_sliderVal, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&imgui_clearColor);
        if (ImGui::Button("Test Window")) imgui_showTestWindow ^= 1;
        if (ImGui::Button("Another Window")) imgui_showAnotherWindow ^= 1;
        ImGui::Separator();
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Frame dt = %.8f sec", frame_dt);
    }

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (imgui_showAnotherWindow){
        ImGui::SetNextWindowSize(ImVec2(200,100), ImGuiSetCond_FirstUseEver);
        ImGui::Begin("Another Window", &imgui_showAnotherWindow);
        ImGui::Text("Hello");
        ImGui::End();
    }

    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
    if (imgui_showTestWindow){
        ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
        ImGui::ShowTestWindow(&imgui_showTestWindow);
    }

    appRM->getShader("cube").use();

    // Draw the Cube
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glActiveTexture(GL_TEXTURE0);
    appRM->getTexture("container").bind();
    appRM->getShader("cube").setInteger("ourTexture1", 0);

    glBindVertexArray(VAO);
    for(GLuint i = 0; i < 10; i++){
        glm::mat4 model;
        model = glm::translate(model, cubePositions[i]);
        GLfloat angle = glm::radians(20.0f)*i;
        
        if(i%3 == 0)
            angle *= glfwGetTime();

        model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
        appRM->getShader("cube").setMatrix4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

	glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}//====================================================


}// END of gui namespace
}// End of astrohelion namespace