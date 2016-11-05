/**
 *  @file Polyline.cpp
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
#include "Polyline.hpp"

namespace astrohelion{
namespace gui{

Polyline::Polyline(){}

Polyline::Polyline(std::vector<float> pts){
	createFromPoints(pts);
}//====================================================

void Polyline::createFromPoints(std::vector<float> pts){
	if(pts.size() < 2*3)
		throw std::runtime_error("Cannot create a polyline with fewer than two points");

	this->points = pts;
	size_t n = pts.size()/3;
	printf("Polyline has %zu points\n", n);

	vertices.clear();
	indices.clear();

	vertices.reserve(7*(n + 2));	// three position elements, four color elements
	indices.reserve(4*n);

	// Begin by creating adjacency point
	glm::vec3 first(pts[0], pts[1], pts[2]);
	glm::vec3 second(pts[3], pts[4], pts[5]);
	glm::vec3 adj_pre = first - glm::normalize(second - first);

	vertices.push_back(adj_pre.x);
	vertices.push_back(adj_pre.y);
	vertices.push_back(adj_pre.z);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);
	vertices.push_back(color[3]);

	for(unsigned int i = 0; i < n; i++){
		vertices.push_back(pts[3*i+0]);
		vertices.push_back(pts[3*i+1]);
		vertices.push_back(pts[3*i+2]);
		vertices.push_back(color[0]);
		vertices.push_back(color[1]);
		vertices.push_back(color[2]);
		vertices.push_back(color[3]);

		if(i < n-1){
			// This point begins a new segment, add the indices of the segment
			// points and adjacency points to the vector
			indices.push_back(i);
			indices.push_back(i+1);
			indices.push_back(i+2);
			indices.push_back(i+3);
		}
	}

	// Append a final adjacency point
	glm::vec3 last(pts[3*n-3], pts[3*n-2], pts[3*n-1]);
	glm::vec3 preLast(pts[3*n-6], pts[3*n-5], pts[3*n-4]);
	glm::vec3 adj_post = last + glm::normalize(last - preLast);

	vertices.push_back(adj_post.x);
	vertices.push_back(adj_post.y);
	vertices.push_back(adj_post.z);
	vertices.push_back(color[0]);
	vertices.push_back(color[1]);
	vertices.push_back(color[2]);
	vertices.push_back(color[3]);

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

		if(i!= 0 && (i+1)%4 == 0)
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

void Polyline::draw(){
	Shader shader = GLOBAL_APP->getResMan()->getShader("line_thick");
	shader.setFloat("thickness", thickness, true);
	shader.setFloat("miterLimit", miterLimit);
	// shader.setVector2f("viewportSize", width, height);
	// shader.setMatrix4("modelViewProjectionMatrix", modelViewProj, true);
	glBindVertexArray(VAO);
	glDrawElements(GL_LINES_ADJACENCY, 4*(points.size()+2), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}//====================================================

void Polyline::setColor(float r, float g, float b, float a){}

/**
 *  @brief Set the line thickness
 *  @param t Line thickness, pixels
 */
void Polyline::setThickness(float t){ thickness = t; }

}	// End of astrohelion namespace
}	// End of gui namespace