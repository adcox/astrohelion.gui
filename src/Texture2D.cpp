/**
 *  @file Texture2D.cpp
 *  @brief 
 *
 *  @author Andrew Cox
 *  Adapted from code found online at <http://www.learnopengl.com/#!In-Practice/2D-Game/Breakout>
 *  @version September 25, 2016
 *  @copyright GNU GPL v3.0
 */

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
#include <iostream>

#include "Texture2D.hpp"

namespace astrohelion{
namespace gui{

/**
 *  @brief Construct a default 2D texture object
 */
Texture2D::Texture2D(){
    glGenTextures(1, &id);
}//====================================================

/**
 *  @brief Generate a texture from image data
 * 
 *  @param w image width, pixels
 *  @param h image height, pixels
 *  @param data image data array
 */
void Texture2D::generate(GLuint w, GLuint h, unsigned char* data){
    width = w;
    height = h;

    // Create Texture
    glBindTexture(GL_TEXTURE_2D, id);
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);

    // Set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMax);
    
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}//====================================================

/**
 *  @brief Binds the texture as the current active GL_TEXTURE_2D texture object
 */
void Texture2D::bind() const{
    glBindTexture(GL_TEXTURE_2D, id);
}//====================================================


}// END of gui namespace
}// END of astrohelion namespace

