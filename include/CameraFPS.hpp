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

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace astrohelion{
namespace gui{

/**
 *  @brief Defines several possible options for camera movement
 *  @details Used as abstraction to stay away from window-system specific
 *  input methods
 * 
 */
enum class CamMove_tp {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

/**
 *  @brief First-Person-Shooter style camera
 *  @details Processes input and calculates the corresponding Euler angles,
 *  vectors and matrices for use in OpenGL
 *  
 *  @author Andrew Cox
 *  @version September 24, 2016
 *  @copyright GNU GPL v3.0
 */
class CameraFPS{

public:
    
    CameraFPS(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f),
            glm::vec3 target = glm::vec3(0.f, 0.f, 0.f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

    void getViewMatrix(glm::mat4*) const;
    glm::mat4 getViewMatrix() const;
    GLfloat getZoom() const;

    void setScreenProperties(float, float, float, float);
    void resetView();

    void processKeyboard(CamMove_tp, GLfloat);
    void processMouseButton(int, int, int);
    void processMouseMovement(GLfloat, GLfloat, int);
    void processMouseScroll(GLfloat);

protected:
    // Camera Attributes
    glm::vec3 position;     //!< Position of the camera in world coordinates
    glm::vec3 target;       //!< Position of object camera is looking at in world coordinates
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);     //!< Direction camera is pointing, unit vector
    glm::vec3 up;           //!< The up direction from the camera's point of view, unit vector
    glm::vec3 right;        //!< The right direction from the camera's point of view, unit vector

    glm::vec3 front_default;   //!< Default front direction
    glm::vec3 up_default;      //!< Default "up" direction
    glm::vec3 right_default;   //!< Default "right" direction
    
    // Screen stuff
    float screenX = 0;  // x-coordinate of camera view area (bottom-left?)
    float screenY = 0;  // y-coordinate of camera view area
    float screenW = 0;  // width of camera view area
    float screenH = 0;  // height of camera view area

    // Camera options
    float moveSpeed = 3.0f;           //!< Camera movement speed, pixels per frame
    float mouseSensitivity = 0.025f;   //!< Scale mouse movement speed
    float mouseScrollScale = 0.075;   //!< Multiple scroll update by this value to adjust speed
    float zoom = 45.0f;       //!< Zoom level, corresponds to field of view, degrees

    bool bMousePressed[3] = {false};    //!< Flags describing which buttons are pressed
    float mousePrevPos[2] = {-1};      //!< Mouse position on screen last frame

    void updateCameraVectors();
};

}// End of Gui namespace
}// End of Astrohelion namespace