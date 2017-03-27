/**
 *  @file MainWindow.hpp
 *	@brief The main astrohelion interface window
 *
 *	@author Andrew Cox
 *	@version November 4, 2016
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

#pragma once

// #include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

#include <imgui/imgui.h>

#include "CameraFPS.hpp"
#include "Shape.hpp"
#include "Polyline.hpp"
#include "Window.hpp"

namespace astrohelion{
namespace gui{

class MainWindow : public Window{
public:
    MainWindow();
    MainWindow(int, int);
    MainWindow(int, int, const char* title, GLFWmonitor *pMonitor = nullptr, Window* share = nullptr);
	~MainWindow();

	void init() override;
	void update() override;
	void draw() override;

    void handleMouseMoveEvent(double, double) override;
protected:

    Polyline line;
    Shape hex;

    float imgui_sliderVal = 0.0f;
    bool imgui_showTestWindow = false;
	bool imgui_showAnotherWindow = false;
	ImVec4 imgui_clearColor = ImColor(114, 144, 154);

    CameraFPS camera;

    GLuint VBO, VAO;
};

}// End of gui namespace
}// End of astrohelion namespace