#version 330 core
out vec4 frag_color;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 object_color;
uniform vec3 light_color;
uniform vec3 light_position;

void main() {
    float ambient_strength = 0.2;
    vec3 ambient = ambient_strength * vec3(1.0);
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light_position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light_color;
    vec3 result = (ambient + diffuse) * object_color;
    frag_color = vec4(result, 1.0);
}