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
#pragma once

#include <GL/glew.h>

namespace astrohelion{
namespace gui{

/**
 *  @brief Texture2D is able to store and configure a texture in OpenGL
 *  @details It also hosts utility functions for easy management
 * 
 *  @author Andrew Cox
 *  Adapted from code found online at <http://www.learnopengl.com/#!In-Practice/2D-Game/Breakout>
 *  @version September 25, 2016
 */
class Texture2D{
public:
    Texture2D();
    
    void Generate(GLuint width, GLuint height, unsigned char* data);
    
    void Bind() const;

    GLuint id = 0;          //!< Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    
    // Texture image dimensions
    GLuint  width = 0,      //!< Width of the loaded image, pixels
            height = 0;     //!< Height of loaded image, pixels
    
    // Texture Format
    GLuint internalFormat = GL_RGB;      //!< Format of texture object
    GLuint imageFormat = GL_RGB;        //!< Format of loaded image
    
    // Texture configuration
    GLuint wrapS = GL_REPEAT;           //!< Wrapping mode on S axis
    GLuint wrapT = GL_REPEAT;           //!< Wrapping mode on T axis
    GLuint filterMin = GL_LINEAR;       //!< Filtering mode if texture pixels < screen pixels
    GLuint filterMax = GL_LINEAR;       //!< Filtering mode if texture pixels > screen pixels
};

    
}// END of gui namespace
}// END of astrohelion namespace