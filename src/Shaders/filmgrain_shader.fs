#version 330 core
out vec4 frag_color;

in vec2 texcoord;

uniform sampler2D u_tex;
uniform bool u_enable;

void main() {
    if (u_enable) {
        vec4 cur_color = texture(u_tex, texcoord);
        float lightness = (cur_color.r + cur_color.g + cur_color.b) / 3;
        frag_color = vec4(lightness, lightness, lightness, 1.0);
    } else {
        frag_color = texture(u_tex, texcoord);
    }
}