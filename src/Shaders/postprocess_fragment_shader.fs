#version 330 core
out vec4 frag_color;

in vec2 texcoord;

uniform sampler2D u_tex;
uniform bool u_enable;

void main() {
    vec4 original_color = texture(u_tex, texcoord);
    if (u_enable) {
        vec4 modified_color;
       
        frag_color = vec4(modified_color);
    } else {
        frag_color = original_color;
    }
}