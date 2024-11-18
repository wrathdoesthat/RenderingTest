#include "shader.hh"

namespace shader {

bool compile_shader(uint32_t& shader_id, const char* src, GLenum shader_type) {
    shader_id = glCreateShader(shader_type);
    glShaderSource(shader_id, 1, &src, NULL);
    glCompileShader(shader_id);

    int success{0};
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader_id, 512, NULL, log);
        std::cout << "Shader compilation failed\n" << log << "\n";

        return false;
    }

    return true;
}

ShaderCreation create(std::filesystem::path shader_path, GLenum shader_type) {
    if (!file_exists(shader_path)) {
        return std::unexpected(CreationErr::FileNotFound);
    }

    uint32_t shader_id{0};
    if (!compile_shader(shader_id, file_to_string(shader_path).str().c_str(), shader_type)) {
        return std::unexpected(CreationErr::CompileFailed);
    }

    return Shader{
        .id = shader_id,
        .type = static_cast<ShaderType>(shader_type),
    };
};

std::vector<ShaderCreation>
create_bulk(std::initializer_list<std::pair<std::filesystem::path, GLenum>> shaders) {
    std::vector<ShaderCreation> results{};
    results.reserve(shaders.size());

    for (auto& item : shaders) {
        if (!file_exists(item.first)) {
            results.push_back(std::unexpected(CreationErr::FileNotFound));
            continue;
        }

        uint32_t shader_id{0};
        if (!compile_shader(shader_id, file_to_string(item.first).str().c_str(), item.second)) {
            results.push_back(std::unexpected(CreationErr::CompileFailed));
            continue;
        }

        results.push_back(Shader{
            .id = shader_id,
            .type = static_cast<ShaderType>(item.second),
        });
    }

    return results;
};
}; // namespace shader