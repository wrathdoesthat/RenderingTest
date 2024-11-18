#pragma once

#include "shader.hh"

namespace shader_program {

struct ShaderProgram {
    uint32_t id{0};
    bool using_standard_uniforms{false};

    operator uint32_t() {
        return id;
    };
};

using ShaderProgramCreation = std::expected<ShaderProgram, GenericErr>;
ShaderProgramCreation create(const shader::Shader& s1, const shader::Shader& s2);

std::vector<ShaderProgramCreation>
create_bulk(std::initializer_list<std::tuple<shader::Shader&, shader::Shader&, bool>> programs);

void use(uint32_t program);

template <typename T>
concept UniformTypes = requires {
    std::is_same_v<T, bool> || std::is_same_v<T, int> || std::is_same_v<T, float> ||
        std::is_same_v<T, glm::mat4> || std::is_same_v<T, glm::vec3>;
};

template <UniformTypes T>
inline void set_uniform(uint32_t program_id, const std::string& name, T value);

template <>
inline void set_uniform<bool>(uint32_t program_id, const std::string& name, bool value) {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), (int)value);
};

template <> inline void set_uniform<int>(uint32_t program_id, const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(program_id, name.c_str()), value);
};

template <>
inline void set_uniform<float>(uint32_t program_id, const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(program_id, name.c_str()), value);
};

template <>
inline void set_uniform<glm::mat4>(uint32_t program_id, const std::string& name, glm::mat4 value) {
    glUniformMatrix4fv(glGetUniformLocation(program_id, name.c_str()), 1, GL_FALSE,
                       glm::value_ptr(value));
};

template <>
inline void set_uniform<glm::vec3>(uint32_t program_id, const std::string& name, glm::vec3 value) {
    glUniform3f(glGetUniformLocation(program_id, name.c_str()), value.x, value.y, value.z);
};

}; // namespace shader_program