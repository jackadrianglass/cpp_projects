#version 330 core
layout(location = 0) out vec4 o_colour;
in vec3 colour;

void main() {
	o_colour = vec4(colour, 1.0);
} 
