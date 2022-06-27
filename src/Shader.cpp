/**
 *  @file Shader.cpp 
 *	@brief OpenGL shader object
 *
 *	@author Andrew Cox
 *	Adapted from code found online at <http://www.learnopengl.com/#!In-Practice/2D-Game/Setting-up>
 *	@version September 25, 2016
 *	@copyright GNU GPL v3.0
 */
 
/*
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

#include "Shader.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "glm/gtc/type_ptr.hpp"

namespace astrohelion{
namespace gui{

//-----------------------------------------------------
//      *structors
//-----------------------------------------------------

Shader::Shader(){}

//-----------------------------------------------------
//      Action Functions
//-----------------------------------------------------

/**
 *  @brief Use the shader program
 *  @details Wrapper for glUseProgram() that applies the 
 *  shader program loaded in this object
 */
Shader& Shader::use(){
	glUseProgram(id);
	return *this;
}//====================================================

void Shader::compile(const GLchar* vertexSource, const GLchar* fragmentSource, const GLchar* geometrySource){
    GLuint sVertex, sFragment, gShader;
    
    // Vertex Shader
    sVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(sVertex, 1, &vertexSource, NULL);
    glCompileShader(sVertex);
    checkCompileErrors(sVertex, "VERTEX");

    // Fragment Shader
    sFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(sFragment, 1, &fragmentSource, NULL);
    glCompileShader(sFragment);
    checkCompileErrors(sFragment, "FRAGMENT");

    // If geometry shader source code is given, also compile geometry shader
    if (geometrySource != nullptr){
        gShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(gShader, 1, &geometrySource, NULL);
        glCompileShader(gShader);
        checkCompileErrors(gShader, "GEOMETRY");
    }

    // Shader Program
    id = glCreateProgram();
    glAttachShader(id, sVertex);
    glAttachShader(id, sFragment);
    if (geometrySource != nullptr)
        glAttachShader(id, gShader);

    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");
    
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(sVertex);
    glDeleteShader(sFragment);
    
    if (geometrySource != nullptr)
        glDeleteShader(gShader);
}//====================================================

//-----------------------------------------------------
//      Set and Get Functions
//-----------------------------------------------------

/**
 *  @brief Retrieve the shader program ID
 *  @return the shader program ID
 */
GLuint Shader::getID() const { return id; }

//-----------------------------------------------------
//      Utility Functions
//-----------------------------------------------------

void Shader::setFloat(const GLchar *name, GLfloat value, GLboolean useShader){
    if (useShader)
        use();
    glUniform1f(glGetUniformLocation(id, name), value);
}//====================================================

void Shader::setInteger(const GLchar *name, GLint value, GLboolean useShader){
    if (useShader)
        use();
    glUniform1i(glGetUniformLocation(id, name), value);
}//====================================================

void Shader::setVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader){
    if (useShader)
        use();
    glUniform2f(glGetUniformLocation(id, name), x, y);
}//====================================================

void Shader::setVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader){
    if (useShader)
        use();
    glUniform2f(glGetUniformLocation(id, name), value.x, value.y);
}//====================================================

void Shader::setVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader){
    if (useShader)
        use();
    glUniform3f(glGetUniformLocation(id, name), x, y, z);
}//====================================================

void Shader::setVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader){
    if (useShader)
        use();
    glUniform3f(glGetUniformLocation(id, name), value.x, value.y, value.z);
}//====================================================

void Shader::setVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader){
    if (useShader)
        use();
    glUniform4f(glGetUniformLocation(id, name), x, y, z, w);
}//====================================================

void Shader::setVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader){
    if (useShader)
        use();
    glUniform4f(glGetUniformLocation(id, name), value.x, value.y, value.z, value.w);
}//====================================================

void Shader::setMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader){
    if (useShader)
        use();
    glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(matrix));
}//====================================================

void Shader::checkCompileErrors(GLuint object, std::string type){
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM"){
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (!success){
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }else{
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (!success){
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
        }
    }
}//====================================================

}// End of gui namespace
}// End of Astrohelion namespace