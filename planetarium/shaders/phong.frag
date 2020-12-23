#version 330 core
out vec4 o_colour;

in vec2 f_tc;
in vec3 f_pos;
in vec3 f_norm;

uniform sampler2D sampler;
uniform vec3 light_pos;
uniform vec3 cam_pos;
uniform vec3 K;
uniform vec3 Ld;
uniform vec3 Ls;
uniform vec3 La;
uniform float alpha;

void main() {
	vec4 d = texture(sampler, f_tc);
	if(d.a < 0.01)
		discard;

	vec3 view = normalize(cam_pos - f_pos);
	vec3 light = normalize(light_pos - f_pos);
	vec3 norm = normalize(f_norm);
    vec3 r = 2 * dot(light, norm) * norm - light;

	// Diffuse
	vec3 Id = K.x * Ld * max(dot(light, norm), 0);

	// Specular
	vec3 Is = K.y * Ls * pow(max(dot(r, view), 0), alpha);

	// Ambient
	vec3 Ia = K.z * La;

	// Total
	vec3 I = Id + Is + Ia;
	o_colour = d * vec4(I, 1);
} 
