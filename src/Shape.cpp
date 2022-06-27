/**
 *  @file Shape.cpp
 *	@brief A class to draw lines from a set of points
 *	@details Several useful resources contributed to this class:
 *	
 *	* http://vicrucann.github.io/tutorials/osg-shader-3dlines/
 *	* https://forum.libcinder.org/topic/smooth-thick-lines-using-geometry-shader
 *	* http://web.engr.oregonstate.edu/~mjb/glman/ClassNotes/geometry_shader.pdf
 *
 *	@author Andrew Cox
 *	@version October 14, 2016
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

#include "GL/glew.h"
#include <glm/glm.hpp>

#include <iostream>
#include <stdexcept>

#include "App.hpp"
#include "ResourceManager.hpp"
#include "Shape.hpp"

namespace astrohelion{
namespace gui{

Shape::Shape(){}

void Shape::createShape(){
	float center[] = {0,0};	// screen coordinates of hexagon center
	float r = 0.2f;				// radius metric
	float theta = 0;			// radians
	double pi = acos(-1.0);

	for(unsigned int i = 0; i < 6; i++){
		vertices.push_back(r*cos(theta));
		vertices.push_back(r*sin(theta));
		vertices.push_back(0.f);
		vertices.push_back(color[0]);
		vertices.push_back(color[1]);
		vertices.push_back(color[2]);
		vertices.push_back(color[3]);

		// Triangles
		if(i > 1){
			indices.push_back(0);
			indices.push_back(i-1);
			indices.push_back(i);
		}

		theta += pi/3;
	}

	printf("Points:\n");
	for(unsigned int i = 0; i < vertices.size(); i++){
		printf("%5.2f", vertices[i]);

		if(i!= 0 && (i+1)%7 == 0)
			printf("\n");
		else
			printf(", ");
	}

	printf("\nIndices:\n");
	for(unsigned int i = 0; i < indices.size(); i++){
		printf("%u", indices[i]);

		if(i!= 0 && (i+1)%3 == 0)
			printf("\n");
		else
			printf(", ");
	}

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &(vertices[0]), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &(indices[0]), GL_STATIC_DRAW);

    // Location 0: Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GL_FLOAT), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Location 1: Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7*sizeof(GL_FLOAT), (GLvoid*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);   // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
    glBindVertexArray(0);   // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
}//====================================================

void Shape::draw(){
	GLOBAL_APP->getResMan()->getShader("colored").use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	// glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}//====================================================

void Shape::setColor(float r, float g, float b, float a){}

} // End of gui namespace
} // End of astrohelion namespace