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

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "GL/glew.h"	// Include GLEW to get all the required OpenGL headers

#include <glm/glm.hpp>

namespace astrohelion{
namespace gui{

// Forward Declarations


/**
 *	@brief A shader object
 *
 *	@author Andrew Cox
 *	@version September 25, 2016
 *	Adpated from code found online at <http://www.learnopengl.com/#!In-Practice/2D-Game/Setting-up>
 *	@copyright GNU GPL v3.0
 */
class Shader{

public:
	//*structors
	Shader();

	// Action functions
	void compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr); // Note: geometry source code is optional 
	Shader& use();

	// Set and Get functions
	GLuint getID() const;
    
    // Utility functions
    void setFloat    (const GLchar*, GLfloat, GLboolean useShader = false);
    void setInteger  (const GLchar*, GLint, GLboolean useShader = false);
    void setVector2f (const GLchar*, GLfloat, GLfloat, GLboolean useShader = false);
    void setVector2f (const GLchar*, const glm::vec2&, GLboolean useShader = false);
    void setVector3f (const GLchar*, GLfloat, GLfloat, GLfloat, GLboolean useShader = false);
    void setVector3f (const GLchar*, const glm::vec3&, GLboolean useShader = false);
    void setVector4f (const GLchar*, GLfloat, GLfloat, GLfloat, GLfloat, GLboolean useShader = false);
    void setVector4f (const GLchar*, const glm::vec4&, GLboolean useShader = false);
    void setMatrix4  (const GLchar*, const glm::mat4&, GLboolean useShader = false);

protected:
	GLuint id = 0;		//!< The program ID
	void checkCompileErrors(GLuint, std::string);
};

}// End of gui namespace
}// End of Astrohelion namespace