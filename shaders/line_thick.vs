#version 330 core

uniform mat4 modelViewProjectionMatrix;

layout(location = 0) in vec3 Vertex;	// Line points; world coordinates
layout(location = 1) in vec4 Color;		// Color of the point

// Output vertex data to the geometry shader
out VertexData{
    vec4 mColor;
} VertexOut;

void main(void){
    VertexOut.mColor = Color;
    gl_Position = modelViewProjectionMatrix * vec4(Vertex, 1);
}