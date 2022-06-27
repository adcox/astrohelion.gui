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
#include "astrohelion/Utilities.hpp"

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
    GLOBAL_APP->getResMan()->loadShader("../shaders/billboard.vert", "../shaders/billboard.frag", "../shaders/hexagon.geom", "billboard");

    // Load the BC4BP system from the file
    // SysData_bc4bp bcSys("../../Astrohelion_scripts/LPF/data/LPF_QH_4B_NaturalManifolds_flyby/Traj019_SEM.mat");
    SysData_bc4bp bcSys("../data/seDPO_37_sp_sem.mat");
    // Load the trajectory from the file
    // Traj_bc4bp natArc("../../Astrohelion_scripts/LPF/data/LPF_QH_4B_NaturalManifolds_flyby/Traj019_SEM.mat", &bcSys);
    Traj_bc4bp natArc("../data/seDPO_37_sp_sem.mat", &bcSys);

    std::vector<float> points, endPts, endPtColors;
    std::vector<float> red {1.f, 0.f, 0.f, 1.f};
    std::vector<float> green {0.f, 1.f, 0.f, 1.f};

    points.reserve(natArc.getNumNodes()*3);
    for(int i = 0; i < natArc.getNumNodes(); i++){
        std::vector<double> state = natArc.getStateByIx(i);
        
        points.push_back(static_cast<float>(state[0]));
        points.push_back(static_cast<float>(state[1]));
        points.push_back(static_cast<float>(state[2]));

        if(i == 0 || i == natArc.getNumNodes()-1){
            endPts.push_back(static_cast<float>(state[0]));
            endPts.push_back(static_cast<float>(state[1]));
            endPts.push_back(static_cast<float>(state[2])); 

            if(i == 0)
                endPtColors.insert(endPtColors.end(), green.begin(), green.end());
            else
                endPtColors.insert(endPtColors.end(), red.begin(), red.end());
        }
    }

    line.createFromPoints(points);
    line.setThickness(2);
    
    bill = BillboardSet(endPts, endPtColors);
    bill.init();

    camera = CameraFPS(glm::vec3(0.0f, 0.0f, 3.f));
    camera.setScreenProperties(0,0, width, height);

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


	// view = camera.getViewMatrix();
    camera.getViewMatrix(&view);
    projection = glm::perspective(camera.getZoom(), (GLfloat)width / (GLfloat)height, 0.1f, 1000.0f);

    GLOBAL_APP->getResMan()->getShader("line_thick").setMatrix4("modelViewProjectionMatrix", projection*view, true);
    GLOBAL_APP->getResMan()->getShader("line_thick").setVector2f("viewportSize", width, height);

    GLOBAL_APP->getResMan()->getShader("billboard").setMatrix4("viewProj", projection*view, true);
    GLOBAL_APP->getResMan()->getShader("billboard").setVector2f("offset", 0, 0);
    GLOBAL_APP->getResMan()->getShader("billboard").setVector2f("viewportSize", width, height);
    GLOBAL_APP->getResMan()->getShader("billboard").setFloat("radius", 20);

    checkForGLErrors("MainWindow::update()");
}//====================================================

void MainWindow::draw(){
    Window::draw();
    
    if(ImGui::Begin("Tools")){
        if(ImGui::CollapsingHeader("Camera")){
            ImGui::RadioButton("Yaw", &cameraOptionRadio, 0); ImGui::SameLine();
            ImGui::RadioButton("Pitch", &cameraOptionRadio, 1); ImGui::SameLine();
            ImGui::RadioButton("Roll", &cameraOptionRadio, 2);

            if(ImGui::Button("Reset View")){
                camera.resetView();
            }
        }
        ImGui::End();
    }

    line.draw();
    bill.draw();

    checkForGLErrors("MainWindow::draw()");
}//====================================================

void MainWindow::handleMouseMoveEvent(double xpos, double ypos){
    Window::handleMouseMoveEvent(xpos, ypos);

    camera.processMouseMovement(xpos, ypos, cameraOptionRadio);
}//====================================================

void MainWindow::handleMouseButtonEvent(int button, int action, int mods){
    Window::handleMouseButtonEvent(button, action, mods);

    camera.processMouseButton(button, action, mods);

    // See if the user clicked on an object

    // Convert all clickable points to screen coordinates
    const std::vector<float>& worldPts = line.getPointsRef();
    std::vector<double> screenPts (worldPts.size(), 0);

    for(unsigned int p = 0; p < worldPts.size()/3; p++){
        glm::vec4 worldPt(worldPts[3*p+0], worldPts[3*p+1], worldPts[3*p+2], 1);
        
        // Project the world point into screen space (centered at (0,0), extents of [-1, 1] in both directions)
        glm::vec4 screenPt = projection*view*worldPt;

        // Shift the origin to the top-left corner
        // Scale by 1/2 so that entire screen has width and height of 1
        // Multiply by width or height to convert to pixels
        // Offset equal to the viewport offset
        screenPt.x = (screenPt.x/screenPt.w + 1.0)/2.0 * width + viewOffset.x;
        screenPt.y = (1.0 - screenPt.y/screenPt.w)/2.0 * height + viewOffset.y;

        screenPts[3*p+0] = screenPt[0];
        screenPts[3*p+1] = screenPt[1];
        screenPts[3*p+2] = 0;
    }

    // saveMatrixToFile("../data/debug/screenPts.mat", "Points", screenPts, screenPts.size()/3, 3);
    printf("Mouse Pos = [%.2f, %.2f]\n", mouse_lastX, mouse_lastY);
    // See if mouse intersects any of the clickable screen coordinates
}//====================================================

void MainWindow::handleWindowSizeEvent(int w, int h){
    Window::handleWindowSizeEvent(w, h);

    camera.setScreenProperties(0,0,w,h);
}//====================================================

}// END of gui namespace
}// End of astrohelion namespace