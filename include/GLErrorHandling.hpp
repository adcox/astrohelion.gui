/*
 *	@file GLErrorHandling.hpp
 *	@brief 
 * 	
 * 	@author Andrew Cox
 * 	@version Sep 19, 2016
 * 	@copyright GNU GPL v3.0
 *	
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

#include <string>
#include <iostream>

namespace astrohelion{
namespace gui{

// Function Declarations
void GLFWErrorCallback(int, const char*);
std::string glErrStr(GLenum);
void checkForGLErrors(const char*);

//-----------------------------------------------------
//      Function Definitions
//-----------------------------------------------------

void GLFWErrorCallback(int err, const char* desc){
	printf("GLFW Error occurred, Error ID: %i, Description: %s\n", err, desc);
}//====================================================


/**
 *  @brief Translate a GLenum error value into a string
 *  @details More details are available at
 *  <https://www.opengl.org/sdk/docs/man2/xhtml/glGetError.xml>
 * 
 *  @param err GLenum error value obtained from glGetError()
 *  @return a descriptive string
 */
std::string glErrStr(GLenum err){
	switch(err){
		case GL_NO_ERROR: return "GL_NO_ERROR: No error has been recorded";
		case GL_INVALID_ENUM: return "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_INVALID_VALUE: return "GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_STACK_OVERFLOW: return "GL_STACK_OVERFLOW: This command would cause a stack overflow. The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_STACK_UNDERFLOW: return "GL_STACK_UNDERFLOW: This command would cause a stack underflow. The offending command is ignored and has no other side effect than to set the error flag.";
		case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
		case GL_TABLE_TOO_LARGE: return "GL_TABLE_TOO_LARGE: The specified table exceeds the implementation's maximum supported table size. The offending command is ignored and has no other side effect than to set the error flag.";
		default: return "Unrecognized GL error enum";
	}
}//====================================================

void checkForGLErrors(const char* msg){
	GLenum error = glGetError();
	while(error != GL_NO_ERROR){
		printf("Error: %s, ErrorID: %i: %s\n", msg, error, glErrStr(error).c_str());
		error = glGetError(); 	// Get next error, if any
	}
}//====================================================
}	// END of gui namespace
}	// END of astrohelion namespace