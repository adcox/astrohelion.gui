/**
 *  @file Billboard.cpp
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

#include "GL/glew.h"
#include <glm/glm.hpp>

#include "assert.h"
#include <iostream>
#include <stdexcept>

#include "App.hpp"
#include "ResourceManager.hpp"
#include "BillboardSet.hpp"

namespace astrohelion{
namespace gui{

BillboardSet::BillboardSet(){}

BillboardSet::BillboardSet(std::vector<float> points, std::vector<float> colors){
	this->points = points;
	this->colors = colors;
	assert(points.size() >= 3);
	assert(colors.size()/4 == points.size()/3);
	numPoints = points.size()/3;
}//====================================================

void BillboardSet::init(){
	if(points.size() < 3)
		return;

	for(unsigned int p = 0; p < numPoints; p++){
		vertexData.insert(vertexData.end(), points.begin() + p*3, points.begin() + (p+1)*3);
		vertexData.insert(vertexData.end(), colors.begin() + p*4, colors.begin() + (p+1)*4);
	}

	for(unsigned int i = 0; i < vertexData.size(); i++){
		printf("%.2f ", vertexData[i]);
	}
	printf("\n");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size()*sizeof(float), &(vertexData[0]), GL_STATIC_DRAW);

    // Location 0: Position (3-d vector)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Location 1: Color (4-d vector)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);   // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0);   // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}//====================================================

void BillboardSet::draw(){
	// GLOBAL_APP->getResMan()->getShader("colored").use();
	GLOBAL_APP->getResMan()->getShader("billboard").use();
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, numPoints);	// Only one point for now
    glBindVertexArray(0);
}//====================================================

} // End of gui namespace
} // End of astrohelion namespace