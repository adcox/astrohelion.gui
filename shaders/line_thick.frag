#version 330 core

// Vertex data from the geometry shader
in VertexData{
    vec2 mTexCoord;
    vec4 mColor;
} VertexIn;

out vec4 color;

void main(void){
	// Apply color data passed in from the program/vertex shader
    color = vec4(VertexIn.mColor.xyz, 1);
}