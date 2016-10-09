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
#include <glm/gtc/matrix_transform.hpp>

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
    
    CameraFPS(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
            GLfloat yaw = -90.0f,
            GLfloat pitch = 0.0f);

    glm::mat4 getViewMatrix() const;
    GLfloat getZoom() const;

    void processKeyboard(CamMove_tp, GLfloat);
    void processMouseMovement(GLfloat, GLfloat, GLboolean constrainPitch = true);
    void processMouseScroll(GLfloat);

protected:
    // Camera Attributes
    glm::vec3 position;     //!< Position of the camera in world coordinates
    glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);     //!< Direction camera is pointing, unit vector
    glm::vec3 up;           //!< The up direction from the camera's point of view, unit vector
    glm::vec3 right;        //!< The right direction from the camera's point of view, unit vector
    glm::vec3 worldUp;      //!< The up direction for the world, unit vector
    
    // Eular Angles
    GLfloat yaw = -90.0f;       //!< Camera yaw angle, degrees
    GLfloat pitch = 0.0f;       //!< Camera pitch angle, degrees

    // Camera options
    GLfloat moveSpeed = 3.0f;   //!< Camera movement speed, pixels per frame
    GLfloat mouseSensitivity = 0.25f;   //!< Scale mouse movement speed
    GLfloat zoom = 45.0f;       //!< Zoom level, corresponds to field of view, degrees

    void updateCameraVectors();
};

}// End of Gui namespace
}// End of Astrohelion namespace