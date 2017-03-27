/**
 *  @file Polyline.hpp
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

#pragma once

#include <vector>

namespace astrohelion{
namespace gui{

class Shape{
public:
	Shape();

	void createShape();
	void draw();

	void setColor(float, float, float, float);
protected:

	std::vector<float> points {};
	std::vector<float> vertices {};
	std::vector<unsigned int> indices {};

	float color[4] = {0.9, 0.9, 0.9, 1.0};

	unsigned int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
};

}	// End of astrohelion namespace
}	// End of gui namespace