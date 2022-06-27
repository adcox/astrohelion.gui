#include "CameraFPS.hpp"

#include "GLFW/glfw3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

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
CameraFPS::CameraFPS(glm::vec3 position, glm::vec3 target, glm::vec3 up){
    this->position = position;
    this->target = target;
    this->up = up;
    
    front = target - position;
    right = glm::normalize(glm::cross(up, -front));

    front_default = front;
    up_default = up;
    right_default = right;
}//====================================================

//-----------------------------------------------------
//      Set and Get Functions
//-----------------------------------------------------

void CameraFPS::getViewMatrix(glm::mat4 *view) const{
    *view = glm::lookAt(position, target, up);
}//====================================================

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 CameraFPS::getViewMatrix() const{

    // printf("Position: [%.1f, %.1f, %.1f]\n", position[0], position[1], position[2]);
    // printf("Front: [%.1f, %.1f, %.1f]\n", front[0], front[1], front[2]);
    // printf("Up: [%.1f, %.1f, %.1f]\n", up[0], up[1], up[2]);
    // printf("Right: [%.1f, %.1f, %.1f]\n\n", right[0], right[1], right[2]);

    return glm::lookAt(position, target, up);
}//====================================================

GLfloat CameraFPS::getZoom() const{ return zoom; }

void CameraFPS::setScreenProperties(float x, float y, float w, float h){
    screenX = x;
    screenY = y;
    screenW = w;
    screenH = h;
}//====================================================

void CameraFPS::resetView(){
    front = front_default;
    position = target - front;
    up = up_default;
    right = right_default;
}//====================================================

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

void CameraFPS::processMouseButton(int button, int action, int mods){
    (void) mods;

    if(button >= 0 && button < 3){
        if(action == GLFW_PRESS){
            bMousePressed[button] = true;

        }else{
            bMousePressed[button] = false;
        }

        // printf("Button Pressed: [%d] [%d] [%d]\n", bMousePressed[0] ? 1 : 0, 
        //     bMousePressed[1] ? 1 : 0, bMousePressed[2] ? 1 : 0);
    }
}//====================================================

/**
 *  @brief Process input from mouse movement
 *  @details [long description]
 * 
 *  @param xPos Mouse x-position, pixels
 *  @param yPos Mouse y-position, pixels
 *  @param axis which axis to rotate around (0 = yaw, 1 = pitch, 2 = roll)
 */
void CameraFPS::processMouseMovement(GLfloat xPos, GLfloat yPos, int axis){

    if(bMousePressed[0]){
        float xOffset = xPos - mousePrevPos[0];
        float yOffset = yPos - mousePrevPos[1];

        // printf("Mouse Offset = [%.2f, %.2f]\n", xOffset, yOffset);

        switch(axis){
            case 0:
                // Yaw about the local "up" direction
                position = glm::rotate(position, -xOffset*static_cast<float>(M_PI/180.f), up);
                front = glm::normalize(target - position);
                right = glm::normalize(glm::cross(up, -front));
                break;
            case 1:
                // Pitch about updated "right" direction
                position = glm::rotate(position, -yOffset*static_cast<float>(M_PI/180.f), right);
                front = glm::normalize(target - position);
                up = glm::normalize(glm::cross(-front, right));
                break;
            case 2:
            {
                float centerX = screenX + 0.5*screenW;
                float centerY = screenY + 0.5*screenH;
                float prevAngle = atan2(-mousePrevPos[1] + centerY, mousePrevPos[0] - centerX);
                float newAngle = atan2(-yPos + centerY, xPos - centerX);
                float diffAngle = newAngle - prevAngle;

                // Avoid big flips (atan2 returns [-pi, pi]) at 180 deg. area
                if(std::abs(diffAngle) > M_PI)
                    diffAngle += -diffAngle/std::abs(diffAngle)*2*M_PI;

                // printf("Prev angle = %.2f, new angle = %.2f\n", prevAngle, newAngle);

                position = glm::rotate(position, diffAngle, front);
                up = glm::rotate(up, diffAngle, front);
                right = glm::normalize(glm::cross(up, -front));
            }
        }
    }

    mousePrevPos[0] = xPos;
    mousePrevPos[1] = yPos;
}//====================================================

/**
 *  @brief Processes input received from a mouse scroll-wheel event
 * 
 *  @param yOffset The change in scroll wheel value during the past frame
 */
void CameraFPS::processMouseScroll(GLfloat yOffset){
    // if (zoom >= 1.0f && zoom <= 45.0f)
    //     zoom -= yoffset*mouseScrollScale;
    
    // if (zoom <= 1.0f)
    //     zoom = 1.0f;
    
    // if (zoom >= 45.0f)
    //     zoom = 45.0f;

    // printf("Zoom = %.2f\n", zoom);

    // Change the distance between the camera and the target
    float dist = glm::length(position) - yOffset*mouseScrollScale;
    if(dist > 0.1f && dist < 10.f){
        position = dist*glm::normalize(position);
    }
}//====================================================

//-----------------------------------------------------
//      Utility Functions
//-----------------------------------------------------


}// END of gui namespace
}// End of astrohelion namespace