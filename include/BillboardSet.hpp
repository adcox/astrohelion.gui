/**
 *  @file BillboardSet.hpp
 *	@brief 
 *
 *	@author Andrew Cox
 *	@version April 18, 2017
 *	@copyright GNU GPL v3.0
 */
 
/*
 *	Astrohelion 
 *	Copyright 2017, Andrew Cox; Protected under the GNU GPL v3.0
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

class BillboardSet{
public:
	BillboardSet();
	BillboardSet(std::vector<float> points, std::vector<float> colors);

	void init();
	void draw();
protected:

	std::vector<float> points {};
	std::vector<float> vertexData {};
	std::vector<float> colors {};

	unsigned int numPoints = 0;		//!< Number of points
	unsigned int VAO = 0;			//!< Vertex array object
	unsigned int VBO = 0;			//!< Vertex buffer object
};

}	// End of astrohelion namespace
}	// End of gui namespace