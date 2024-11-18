#version 330 core
out vec4 frag_color;

in vec3 color;
in vec2 texcoord;

uniform sampler2D u_tex1;
uniform sampler2D u_tex2;
uniform float u_bias;

void main() {
    frag_color =
        mix(texture(u_tex1, texcoord), texture(u_tex2, texcoord), u_bias);
}