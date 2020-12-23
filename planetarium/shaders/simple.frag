#version 330 core
out vec4 o_colour;

in vec2 f_tc;

uniform sampler2D sampler;

void main() {
    vec4 d = texture(sampler, f_tc);
    if(d.a < 0.01)
        discard;

    o_colour = d;
}