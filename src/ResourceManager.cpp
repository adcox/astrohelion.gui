/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "ResourceManager.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <fstream>

#include <SOIL/SOIL.h>

namespace astrohelion{
namespace gui{

ResourceManager::ResourceManager(){
    // Initializations? None so far
}//====================================================

ResourceManager::~ResourceManager(){
    std::cout << "ResourceManager::~ResourceManager" << std::endl;
    clear();
}//====================================================

void ResourceManager::addShader(std::string name, Shader shader){
    shaders[name] = shader;
}//====================================================

void ResourceManager::addTexture(std::string name, Texture2D tex){
    textures[name] = tex;
}//====================================================

/**
 *  @brief Loads and generates a shader program from file
 *  @details A vertex, fragment
 * 
 *  @param vShaderFile filepath to vertex shader source code
 *  @param fShaderFile filepath to fragment shader source code
 *  @param gShaderFile filepath to geometry shader source file (optional; set to nullptr to skip loading the geometry shader)
 *  @param name descriptive name to identify this shader
 *  @return The generated shader program
 */
Shader& ResourceManager::loadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name){
    shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return shaders[name];
}//====================================================

/**
 *  @brief Retrieve a pre-loaded shader
 * 
 *  @param name Descriptive name for the shader
 *  @return The shader object
 */
Shader& ResourceManager::getShader(std::string name){
    if(shaders.count(name))
        return shaders[name];
    else
        throw std::runtime_error("ResourceManager::getShader: invalid name");
}//====================================================

/**
 *  @brief Load a texture from file
 * 
 *  @param file filepath to texture image
 *  @param alpha whether or not the texture has transparency values
 *  @param name Descriptive name for later reference
 *  @return The loaded texture
 */
Texture2D& ResourceManager::loadTexture(const GLchar *file, GLboolean alpha, std::string name){
    textures[name] = loadTextureFromFile(file, alpha);
    return textures[name];
}//====================================================

/**
 *  @brief Retrieve a pre-loaded texture
 * 
 *  @param name Descriptive name for the texture
 *  @return The texture object
 */
Texture2D& ResourceManager::getTexture(std::string name){
    if(textures.count(name))
        return textures[name];
    else
        throw std::runtime_error("ResourceManager::getTexture: invalid name");
}//====================================================

/**
 *  @brief Properly deallocates all loaded resources
 */
void ResourceManager::clear(){
    std::cout << "ResourceManager::clear" << std::endl;
    // (Properly) delete all shaders	
    for (auto iter : shaders)
        glDeleteProgram(iter.second.getID());
    shaders.clear();

    // (Properly) delete all textures
    for (auto iter : textures)
        glDeleteTextures(1, &iter.second.id);
    textures.clear();
}//====================================================

/**
 *  @brief Load a shader from file
 * 
 *  @param vShaderFile filepath to vertex shader source code
 *  @param fShaderFile filepath to fragment shader source code
 *  @param gShaderFile filepath to geometry shader source file (optional; set to nullptr to skip loading the geometry shader)
 *  @return The generated shader program
 */
Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile){
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try{
        // Open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        
        // Read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        
        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        
        // If geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr){
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e){
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    const GLchar *gShaderCode = geometryCode.c_str();
    
    // 2. Now create shader object from source code
    Shader shader;
    shader.compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}//====================================================

/**
 *  @brief Load a texture from file
 * 
 *  @param file filepath to texture image
 *  @param alpha whether or not the texture has alpha values
 * 
 *  @return The texture object
 */
Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha){
    // Create Texture object
    Texture2D texture;
    if (alpha){
        texture.internalFormat = GL_RGBA;
        texture.imageFormat = GL_RGBA;
    }

    // Load image
    int width, height;
    unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.imageFormat == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
    
    // Now generate texture
    texture.generate(width, height, image);
    
    // And finally free image data
    SOIL_free_image_data(image);
    return texture;
}//====================================================

}// END of gui namespace
}// END of astrohelion namespace