/*
 *	@file GLErrorHandling.hpp
 *	@brief 
 * 	
 * 	@author Andrew Cox
 * 	@version Sep 19, 2016
 * 	@copyright GNU GPL v3.0
 *	
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

#include "GL/glew.h"		// This header must be included BEFORE glfw3
#include <string>

namespace astrohelion{
namespace gui{

// Function Declarations
void GLFWErrorCallback(int, const char*);
std::string glErrStr(GLenum);
void checkForGLErrors(const char*);

}	// END of gui namespace
}	// END of astrohelion namespace