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

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>

namespace astrohelion{
namespace gui{
// Forward Declarations


struct FontChar{
	GLuint texID = 0;		//!< ID associated with texture for this glyph
	glm::ivec2 size;		//!< Width and Height of the character, pixels
	glm::ivec2 bearing;		//!< horizontal and vertical bearing (position relative to origin/baseline), pixels
	GLuint advance = 0;		//!< horizontal advance to next glyph, in 1/64 pixels (face->glyph->advance.x)
};

/**
 *	@brief Load fonts and render text
 *
 *	@author Andrew Cox
 *	@version September 22, 2016
 *	@copyright GNU GPL v3.0
 */
class Font{

public:
	Font();
	Font(GLFWwindow*);
	Font(const Font&);

	const Font& operator =(const Font&);

	void initFont(const char*, float);
	void renderText(std::string, GLfloat, GLfloat);
	void renderText(std::string, GLfloat, GLfloat, GLfloat, glm::vec3);

	void updateWindow(GLFWwindow*);

protected:
	void copyMe(const Font&);
	void init();
	void initProjection();

	GLuint VAO = 0;		//!< Vertex array for font data
	GLuint VBO = 0;		//!< Vertex buffer for font data

	int viewW = 800;	//!< Width of the viewport, pixels
	int viewH = 600;	//!< Height of the viewport, pixels

	std::map<GLchar, FontChar> characters {};	//!< Map of data objects that store the texture ID and size of various glyphs
private:

};

}// End of Gui namespace
}// End of Astrohelion namespace