#include "GLErrorHandling.hpp"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

namespace astrohelion{
namespace gui{

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