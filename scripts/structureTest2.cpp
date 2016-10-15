/**
 * Test the architecture / structure of my OpenGL setup
 */

#include <iostream>
#include <stdexcept>

#include "App.hpp"
#include "Window.hpp"
#include "DemoWindow.hpp"

namespace astroGui = astrohelion::gui;
using namespace astroGui;

App app;

int main(){

	app.init();

	// Create Windows
	Window* win1 = app.createWindow(500, 500, "Window 1");
	app.setMainWindow(win1);

	// Create a window and share resources with win1
	// If resources are not shared, then we need to load default resources for each
	// and every window that is created; how to do this without overwriting names
	// for textures and shaders could be a challenge.
	app.createWindow(500, 500, "Window 2", nullptr, win1);

	// Load some textures and shaders for the demo windwos
	app.getResMan()->loadShader("../shaders/texture3D.vs", "../shaders/textured.frag", nullptr, "cube");
	app.getResMan()->loadTexture("../textures/container.jpg", false, "container");


	app.run();

	return EXIT_SUCCESS;
}//====================================================