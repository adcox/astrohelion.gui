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

#include "MainWindow.hpp"

#include <cstdio>

#include "App.hpp"
#include "GLErrorHandling.hpp"
#include "ResourceManager.hpp"

// Includes for Astrohelion Demo
#include "astrohelion/SysData_bc4bp.hpp"
#include "astrohelion/Traj_bc4bp.hpp"

namespace astrohelion{
namespace gui{

//-----------------------------------------------------
//      *structors
//-----------------------------------------------------
MainWindow::MainWindow(){}
MainWindow::MainWindow(int w, int h) : Window(w, h) {}
MainWindow::MainWindow(int w, int h, const char* title, GLFWmonitor *pMonitor, Window* share) : Window(w, h, title, pMonitor, share) {}

MainWindow::~MainWindow(){
	if(pWindow){
		glDeleteVertexArrays(1, &VAO);
    	glDeleteBuffers(1, &VBO);
	}
    checkForGLErrors("MainWindow::~MainWindow()");
}//====================================================

void MainWindow::init(){
    Window::init();

    if(!GLOBAL_APP->getResMan()){
        throw std::runtime_error("MainWindow::init: Resource Manager has not been loaded; cannot init window");
    }

    // Load the BC4BP system from the file
    // SysData_bc4bp bcSys("../../Astrohelion_scripts/LPF/data/LPF_QH_4B_NaturalManifolds_flyby/Traj019_SEM.mat");
    SysData_bc4bp bcSys("../data/se_dpo_sem.mat");
    // Load the trajectory from the file
    // Traj_bc4bp natArc("../../Astrohelion_scripts/LPF/data/LPF_QH_4B_NaturalManifolds_flyby/Traj019_SEM.mat", &bcSys);
    Traj_bc4bp natArc("../data/se_dpo_sem.mat", &bcSys);

    std::vector<float> points;
    points.reserve(natArc.getNumNodes()*3);
    for(int i = 0; i < natArc.getNumNodes(); i++){
        std::vector<double> state = natArc.getStateByIx(i);
        
        points.push_back(static_cast<float>(state[0]));
        points.push_back(static_cast<float>(state[1]));
        points.push_back(static_cast<float>(state[2]));
    }

    line.createFromPoints(points);
    line.setThickness(1);
    
    hex.createShape();

    camera = CameraFPS(glm::vec3(0.0f, 0.0f, 3.f));
    checkForGLErrors("MainWindow::init()");
}//====================================================

void MainWindow::update(){
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

    GLOBAL_APP->getResMan()->getShader("line_thick").setMatrix4("modelViewProjectionMatrix", projection*view, true);
    GLOBAL_APP->getResMan()->getShader("line_thick").setVector2f("viewportSize", width, height);

    checkForGLErrors("MainWindow::update()");
}//====================================================

void MainWindow::draw(){
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

    // line.draw();
    hex.draw();
    checkForGLErrors("MainWindow::draw()");
}//====================================================

void MainWindow::handleMouseMoveEvent(double xpos, double ypos){
    Window::handleMouseMoveEvent(xpos, ypos);

    // camera.processMouseMovement(xpos, ypos);
}//====================================================

}// END of gui namespace
}// End of astrohelion namespace