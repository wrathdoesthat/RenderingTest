#version 330 core
layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec2 ptexcoord;

out vec2 texcoord;

void main() {
    gl_Position = vec4(vertex_pos.x, vertex_pos.y, vertex_pos.z, 1.0);
    texcoord = ptexcoord;
}