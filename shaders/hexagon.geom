#version 330
/**
 *  \brief This shader draws a hexagon each vertex in the set
 *  \details Vertice coordinates must be in normalized screen coordinates, i.e.
 *  in the range [-1, 1]. The hexagon primitive is drawn in the Screen's xy-plane
 *  and any x- or w-coordinates are overwritten.
 */

uniform vec2 viewportSize;	//!< Viewport size, pixels
uniform float radius;		//!< Hexagon radius, pixels

layout(points) in;			// Individual points are fed in
layout(triangle_strip, max_vertices=9) out;	// A triangle strip with 9 points is output

// Data passed in from the vertex shader
in VertexData{
    vec4 mColor;
} VertexIn[];

// Data passed out to the fragment shader
out VertexData{
    vec4 mColor;
} VertexOut;

const float PI = 3.1415926;

/**
 *  \brief Convert from normalized screen coordinates to
 *  flat, pixel screen coordinates
 * 
 *  \param vertex A 4-d vertex vector (x, y, z, w)
 *  \return a 2-d vertex position on the screen
 */
vec2 toScreenSpace(vec4 vertex){
    return vec2( vertex.xy ) * viewportSize;
}

void main(){

	// Only one color for the entire polygon
	VertexOut.mColor = VertexIn[0].mColor;	// only one vertex input, so index at 0
	
	// Convert the input coordinate to pixels
	vec2 center = toScreenSpace(gl_in[0].gl_Position);

	// First point: left of center
	gl_Position = vec4(center.x-radius, center.y, 0.0, 1.0);
	gl_Position.xy /= viewportSize;
	EmitVertex();

	// Next: up, CW
	gl_Position = vec4(center.x - 0.5*radius, center.y + (sqrt(3)/2.0)*radius, 0.0, 1.0);
	gl_Position.xy /= viewportSize;
	EmitVertex();

	// Center
	gl_Position = vec4(center/viewportSize, 0.0, 1.0);
	EmitVertex();

	// Top-right
	gl_Position = vec4(center.x + 0.5*radius, center.y + (sqrt(3)/2.0)*radius, 0.0, 1.0);
	gl_Position.xy /= viewportSize;
	EmitVertex();

	// Right
	gl_Position = vec4(center.x + radius, center.y, 0.0, 1.0);
	gl_Position.xy /= viewportSize;
	EmitVertex();

	// Bottom-right
	gl_Position = vec4(center.x + 0.5*radius, center.y - (sqrt(3)/2.0)*radius, 0.0, 1.0);
	gl_Position.xy /= viewportSize;
	EmitVertex();

	// Center again so that shape is filled completely
	gl_Position = vec4(center/viewportSize, 0.0, 1.0);
	EmitVertex();

	// Bottom-left
	gl_Position = vec4(center.x - 0.5*radius, center.y - (sqrt(3)/2.0)*radius, 0.0, 1.0);
	gl_Position.xy /= viewportSize;
	EmitVertex();

	// Left again, to complete shape
	gl_Position = vec4(center.x-radius, center.y, 0.0, 1.0);
	gl_Position.xy /= viewportSize;
	EmitVertex();

	EndPrimitive();
}