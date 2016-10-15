/**
 *  @file Font.cpp
 *	@brief Load and render fonts
 *
 *	@author Andrew Cox
 *	@version September 22, 2016
 *	@copyright GNU GPL v3.0
 *	
 *	Adapted from TrueType Font tutorial found at
 *	http://www.learnopengl.com/#!In-Practice/Text-Rendering
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
#include "Font.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>


namespace astrohelion{
namespace gui{

//-----------------------------------------------------
//      *structors
//-----------------------------------------------------

/**
 *  @brief Construct a default Font
 */
Font::Font(){
	init();
}//====================================================

/**
 *  @brief Construct a font in a given window
 *  @param pWindow pointer to the GLFW window
 */
Font::Font(GLFWwindow *pWindow, std::shared_ptr<ResourceManager> rm){
	glfwGetWindowSize(pWindow, &viewW, &viewH);
	appRM = rm;

	init();
}//====================================================

/**
 *  @brief Copy the Font object
 * 
 *  @param fm reference to another font object
 */
Font::Font(const Font &fm){
	copyMe(fm);
}//====================================================

/**
 *  @brief Initialize the font, no dependence on font face or size here
 */
void Font::init(){
	if(appRM){
		appRM->loadShader("../shaders/text.vs", "../shaders/text.frag", nullptr, "font");
	}

	initProjection();

	// Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}//====================================================

//-----------------------------------------------------
//      Operators
//-----------------------------------------------------

/**
 *  @brief Copy the Font
 * 
 *  @param fm [description]
 *  @return [description]
 */
const Font& Font::operator =(const Font &fm){
	copyMe(fm);
	return *this;
}//====================================================

//-----------------------------------------------------
//      Set and Get Functions
//-----------------------------------------------------

/**
 *  @brief Update the size of the viewport
 *  @details If the constructor is used that accepts a GLFW window
 *  pointer, the size of the window is set initially. Otherwise,
 *  use this function to set or update the window
 * 
 *  @param pWindow pointer to the GLFW window
 */
void Font::updateWindow(GLFWwindow *pWindow){
	glfwGetWindowSize(pWindow, &viewW, &viewH);
	initProjection();
}//====================================================

//-----------------------------------------------------
//      Utility Functions
//-----------------------------------------------------

/**
 *  @brief Initialize an orthographic projection matrix
 *  @details This matrix is used in the vertex and fragment shaders to render
 *  the font glyphs in an orthorgraphic projection on screen
 */
void Font::initProjection(){
	if(appRM){
		// Compute the projection matrix and setup the shader to use it
		glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(viewW), 0.0f, static_cast<GLfloat>(viewH));
		appRM->getShader("font").setMatrix4("projection", projection, true);
	}
}//====================================================

/**
 *  @brief Initialize the font
 *  @details [long description]
 * 
 *  @param fontPath Filepath to the font TTF file
 *  @param size Size of the font, pixels (I think?)
 */
void Font::initFont(const char* fontPath, float size){
	// All functions return 0 if the succeed, something else otherwise
	FT_Library library;
	if(FT_Init_FreeType(&library))
		throw std::runtime_error("Font: Could not init FreeType Library");

	// Load font as face
	FT_Face face;
	if(FT_New_Face(library, fontPath, 0, &face))
		throw std::runtime_error("Font::initFont: Failed to load FreeType font");

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, size);	// Set width to zero so it is auto-computed

	// Disable byte-alignment restriction (allow storing in one color value)
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for(GLubyte c = 0; c < 128; c++){
		// Load character glyph
		if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
			std::cout << "ERROR::FREETYPE: Failed to load glyph, ascii code " << c << std::endl;
			continue;
		}

		// Generate texture
		FontChar fchar;
		glGenTextures(1, &(fchar.texID));
		glBindTexture(GL_TEXTURE_2D, fchar.texID);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		); // single byte of data (8 bits) is stored in RED component of color

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Store character for later use
		fchar.size = glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows);
		fchar.bearing = glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top);
		fchar.advance = static_cast<GLuint>(face->glyph->advance.x);
		// FontChar fchar = {
		// 	texture,
		// 	glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
		// 	glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
		// 	static_cast<GLuint>(face->glyph->advance.x)
		// };

		characters.insert(std::pair<GLchar, FontChar>(c, fchar));
	}

	glBindTexture(GL_TEXTURE_2D, 0);	// Unset the texture

	// Destroy face once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(library);
}//====================================================

/**
 *  @brief Render some text on screen
 *  @details [long description]
 * 
 *  @param str Text to render
 *  @param x bottom-left corner of the text, pixels, screen coord
 *  @param y bottom-left corner of the text, pixels, screen coord
 */
void Font::renderText(std::string str, GLfloat x, GLfloat y){
	// Default to scale = 1 and color = black
	renderText(str, x, y, 1.0f, glm::fvec3(0.0, 0.0, 0.0));
}//====================================================

/**
 *  @brief Render some text on screen
 *  @details [long description]
 * 
 *  @param str Text to render
 *  @param x bottom-left corner of the text, pixels, screen coord
 *  @param y bottom-left corner of the text, pixels, screen coord
 *  @param scale scaling factor for the font
 *  @param color RGB normalized color vector (values 0 - 1)
 */
void Font::renderText(std::string str, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color){
	// Set OpenGL options
	// TODO: Detect old values and rest after rendering text??
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate the corresponding render state
	if(appRM){
		appRM->getShader("font").setVector3f("textColor", color.x, color.y, color.z, true);
	}
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iteration through all characters
	std::string::const_iterator c;
	for(c = str.begin(); c != str.end(); c++){
		FontChar ch = characters[*c];

		GLfloat xpos = x + ch.bearing.x * scale;
		GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;

		// Update VBO for each character
		GLfloat verts[6][4] = {
			{ xpos, 	ypos + h, 	0.0, 0.0 },
			{ xpos, 	ypos, 		0.0, 1.0 },
			{ xpos + w, ypos, 		1.0, 1.0 },

			{ xpos, 	ypos + h, 	0.0, 0.0 },
			{ xpos + w, ypos, 		1.0, 1.0 },
			{ xpos + w, ypos + h, 	1.0, 0.0 }
		};

		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.texID);

		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verts), verts);	// Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale;	// Bitshift by 6 to get value in pixels (2^6 = 64; divide amount of 1/64th pixels to get number of pixels)
	}

	glBindVertexArray(0);				// Unset VAO
	glBindTexture(GL_TEXTURE_2D, 0);	// Unset texture
}//====================================================

/**
 *  @brief Copy the font object
 *  @param fm Reference to another font object
 */
void Font::copyMe(const Font &fm){
	ResourceUser::copyMe(fm);
	VAO = fm.VAO;
	VBO = fm.VBO;
	viewW = fm.viewW;
	viewH = fm.viewH;
	characters = fm.characters;
}//====================================================

}// End of gui namespace
}// End of astrohelion namespace