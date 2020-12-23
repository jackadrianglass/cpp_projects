#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 norm;

out vec2 f_tc;
out vec3 f_pos;
out vec3 f_norm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	f_tc = texCoord;
	f_norm = vec3(model * vec4(norm, 0.0));
	f_pos = vec3(model * vec4(pos, 1.0));

	gl_Position = projection * view * model * vec4(pos, 1.0);
}
