#pragma once

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#undef GLFW_INCLUDE_NONE

#include "glad/glad.h"
#include "utils.hh"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <expected>
#include <filesystem>
#include <iostream>
#include <type_traits>
#include <vector>

namespace shader {

enum class ShaderType {
    Vertex = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER
};

struct Shader {
    uint32_t id{0};
    ShaderType type{ShaderType::Vertex};

    operator uint32_t() {
        return id;
    };
};

enum class CreationErr {
    CompileFailed,
    FileNotFound
};

using ShaderCreation = std::expected<Shader, CreationErr>;

ShaderCreation create(std::filesystem::path shader_path, GLenum shader_type);
std::vector<ShaderCreation>
create_bulk(std::initializer_list<std::pair<std::filesystem::path, GLenum>> shaders);
} // namespace shader