#include "CameraFPS.hpp"

#include <iostream>
#include <cstdio>

namespace astrohelion{
namespace gui{

//-----------------------------------------------------
//      *structors
//-----------------------------------------------------


/**
 *  @brief Construct a first-person-shooter type camera
 *  @details [long description]
 * 
 *  @param position Position of the camera in world coordinates
 *  @param up Up direction from the camera's point of view, unit vector
 *  @param yaw Initial yaw value, degrees
 *  @param pitch Initial pitch value, degrees
 */
CameraFPS::CameraFPS(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch){
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;

    updateCameraVectors();
}//====================================================

//-----------------------------------------------------
//      Set and Get Functions
//-----------------------------------------------------

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 CameraFPS::getViewMatrix() const{

    // printf("Position: [%.1f, %.1f, %.1f]\n", position[0], position[1], position[2]);
    // printf("Front: [%.1f, %.1f, %.1f]\n", front[0], front[1], front[2]);
    // printf("Up: [%.1f, %.1f, %.1f]\n", up[0], up[1], up[2]);
    // printf("Right: [%.1f, %.1f, %.1f]\n\n", right[0], right[1], right[2]);

    return glm::lookAt(position, position + front, up);
}//====================================================

GLfloat CameraFPS::getZoom() const{ return zoom; }

//-----------------------------------------------------
//      Event Handler Functions
//-----------------------------------------------------

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void CameraFPS::processKeyboard(CamMove_tp direction, GLfloat deltaTime){
    GLfloat velocity = moveSpeed * deltaTime;

    switch (direction){
    	case CamMove_tp::FORWARD: position += front * velocity; break;
    	case CamMove_tp::BACKWARD: position -= front * velocity; break;
    	case CamMove_tp::LEFT: position -= right * velocity; break;
    	case CamMove_tp::RIGHT: position += right * velocity; break;
    }
}//====================================================

/**
 *  @brief Process input from mouse movement
 *  @details [long description]
 * 
 *  @param xoffset Change in mouse x-position during the past frame, pixels
 *  @param yoffset Change in mouse y-position during the past frame, pixels
 *  @param constrainPitch Whether or not to limit the pitch value to [-89, 89] degrees
 */
void CameraFPS::processMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch){
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw   += xoffset;
    pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch){
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // Update front, right and up Vectors using the updated Eular angles
    updateCameraVectors();
}//====================================================

/**
 *  @brief Processes input received from a mouse scroll-wheel event
 * 
 *  @param yoffset The change in scroll wheel value during the past frame
 */
void CameraFPS::processMouseScroll(GLfloat yoffset){
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yoffset*mouseScrollScale;
    
    if (zoom <= 1.0f)
        zoom = 1.0f;
    
    if (zoom >= 45.0f)
        zoom = 45.0f;

    printf("Zoom = %.2f\n", zoom);
}//====================================================

//-----------------------------------------------------
//      Utility Functions
//-----------------------------------------------------

/**
 *  @brief Calculate the front vector from the camera's (updated)
 *  Euler angles
 */
void CameraFPS::updateCameraVectors(){
    // Calculate the new front vector
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front);
    // Also re-calculate the right and up vector
    right = glm::normalize(glm::cross(front, worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    up    = glm::normalize(glm::cross(right, front));
}//================================================


}// END of gui namespace
}// End of astrohelion namespace