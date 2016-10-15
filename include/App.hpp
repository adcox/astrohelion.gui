/*
 *  Astrohelion 
 *  Copyright 2016, Andrew Cox; Protected under the GNU GPL v3.0
 *  
 *  This file is part of Astrohelion
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

#include <list>
#include <memory>

// external Forward declarations
class GLFWmonitor;

namespace astrohelion{
namespace gui{

// astrohelion::gui Forward declarations
class ResourceManager;
class Window;

/**
 *  @brief Application
 *  @details [long description]
 *  @return [description]
 */
class App{
public: 
	App();
	virtual ~App();

	Window* createWindow(int width, int height, const char* title, GLFWmonitor* pMonitor = nullptr, Window* share = nullptr);
	void makeContextCurrent(Window*);

	Window* getMainWindow() const;
	std::shared_ptr<ResourceManager> getResMan() const;

	void setMainWindow(Window*);

	void init();
	void run();

protected:
	Window* mainWindow = nullptr;
	Window* currentWindow = nullptr;
	std::shared_ptr<ResourceManager> resourceMan = nullptr;

	std::list<Window*> windows;

	unsigned int nextWinID = 0;

	bool shouldClose();


};

extern App* GLOBAL_APP;


}	// END of gui namespace
}	// END of astrohelion namespace