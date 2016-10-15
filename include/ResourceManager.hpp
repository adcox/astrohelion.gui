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

#include <map>
#include <string>

#include <GL/glew.h>

#include "Texture2D.hpp"
#include "Shader.hpp"

namespace astrohelion{
namespace gui{

/**
 *  @brief A ResourceManager class that hosts several
 *  functions to load textures and shaders.
 *  @details Each loaded texture or shader is also stored for future
 *  reference by string handles.
 *  
 *  @author Andrew Cox
 *  Adapted from code found online at <http://www.learnopengl.com/#!In-Practice/2D-Game/Breakout>
 *  @version September 25, 2016
 *  @copyright GNU GPL v3.0
 */
class ResourceManager{

public:

    ResourceManager();
    ~ResourceManager();

    void addShader(std::string, Shader);
    void addTexture(std::string, Texture2D);
    
    Shader& getShader(std::string);
    Texture2D& getTexture(std::string);

    Shader& loadShader(const GLchar*, const GLchar*, const GLchar*, std::string);
    Texture2D& loadTexture(const GLchar*, GLboolean, std::string);
    
    void clear();
protected:
    // Resource storage
    std::map<std::string, Shader>    shaders;
    std::map<std::string, Texture2D> textures;

    // Loads and generates a shader from file
    Shader loadShaderFromFile(const GLchar*, const GLchar*, const GLchar *gShaderFile = nullptr);

    // Loads a single texture from file
    Texture2D loadTextureFromFile(const GLchar*, GLboolean);
};


}// END of gui namespace
}// END of astrohelion namespace