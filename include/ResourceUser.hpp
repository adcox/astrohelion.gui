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

#include "ResourceManager.hpp"
#include <memory>

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
class ResourceUser{

public:
    ResourceUser(){}
    virtual ~ResourceUser(){}

    void setResourceManager(std::shared_ptr<ResourceManager> rm){ appRM = rm; }
protected:
    std::shared_ptr<ResourceManager> appRM;

    /**
     *  @brief Copy the resource manager smart pointer to a new object
     *  @details Any class that extends this interface should call 
     *  the copyMe() function in their own copyMe() functions (if they exist)
     * 
     *  @param ru Reference to another ResourceUser object
     */
    virtual void copyMe(const ResourceUser &ru){
        appRM = ru.appRM;
    }//================================================
};


}// END of gui namespace
}// END of astrohelion namespace