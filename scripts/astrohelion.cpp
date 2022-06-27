/**
 * Test Astrohelion Links
 */

#include <iostream>
#include <stdexcept>

#include "App.hpp"
#include "DemoWindow.hpp"
#include "MainWindow.hpp"
#include "ResourceManager.hpp"
#include "Window.hpp"

namespace astro = astrohelion;
namespace astroGui = astrohelion::gui;

astroGui::App app;

int main(){

	app.init();

	// Create Windows
	astroGui::MainWindow* mainWin = new astroGui::MainWindow(1080, 700);
	if(app.addWindow(mainWin, "Astrohelion")){
		app.setMainWindow(mainWin);
	}else{
		mainWin = nullptr;
		return EXIT_FAILURE;
	}

	// Create a window and share resources with win1
	// If resources are not shared, then we need to load default resources for each
	// and every window that is created; how to do this without overwriting names
	// for textures and shaders could be a challenge.
	app.addWindow(new astroGui::DemoWindow(500, 500), "Window 2", nullptr, mainWin);

	// Load some textures and shaders for the demo windwos
	app.getResMan()->loadShader("../shaders/texture3D.vs", "../shaders/textured.frag", nullptr, "cube");
	app.getResMan()->loadTexture("../textures/container.jpg", false, "container");


	app.run();

	return EXIT_SUCCESS;
}//====================================================