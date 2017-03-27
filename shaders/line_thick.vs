#version 330 core

uniform mat4 modelViewProjectionMatrix;

layout(location = 0) in vec3 Vertex;
layout(location = 1) in vec3 Color;

// Output vertex data to the geometry shader
out VertexData{
    vec4 mColor;
} VertexOut;

void main(void)
{
    // VertexOut.mColor = Color;
    gl_Position = modelViewProjectionMatrix * vec4(Vertex, 1);
    VertexOut.mColor = vec4(Color, 1);
    // gl_Position = vec4(Vertex, 1);
}