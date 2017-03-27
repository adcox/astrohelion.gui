#version 330 core

in vec3 vertColor;

out vec4 color;

void main(){
	// color = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f);
	color = vec4(vertColor, 1.0f);
}