#version 330 core

layout (location = 0) in vec3 Vertex; 	//!< Position of the billboard, world coordinates
layout (location = 1) in vec4 Color;	//!< Color of the billboard
  
uniform mat4 viewProj;				//!< The View-Projection matrix
uniform vec2 viewportSize;			//!< Size of the viewport, pixels
uniform vec2 offset;				//!< Offset from Vertex in pixels

out VertexData{
    vec4 mColor;
} VertexOut;

void main(){

	// Transform world coordinates to screen coordinates (we don't apply a model matrix)
	gl_Position = viewProj * vec4(Vertex, 1.0);

	// Divide by w so that scale of object remains constant
	gl_Position /= gl_Position.w;

	// Translate vertex according to offset
	gl_Position.xy += offset/viewportSize;	// divide by viewportSize to convert pixels to normalized coordinates

    VertexOut.mColor = Color; // Set mColor to the input color we got from the vertex data
}