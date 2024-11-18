#include "shader_program.hh"

namespace shader_program {
bool link_program(uint32_t& program_id, const shader::Shader& s1, const shader::Shader& s2) {
    program_id = glCreateProgram();
    glAttachShader(program_id, s1.id);
    glAttachShader(program_id, s2.id);
    glLinkProgram(program_id);

    int success{0};
    glGetProgramiv(program_id, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(program_id, 512, NULL, log);
        std::cout << "Shader program link failed\n" << log << "\n";

        return false;
    }

    return true;
}

ShaderProgramCreation create(const shader::Shader& s1, const shader::Shader& s2,
                             bool use_standard_uniforms) {

    uint32_t program_id{0};
    if (!link_program(program_id, s1, s2)) {
        return std::unexpected(GenericErr::Fail);
    }

    return ShaderProgram{.id = program_id, .using_standard_uniforms = use_standard_uniforms};
};

std::vector<ShaderProgramCreation>
create_bulk(std::initializer_list<std::tuple<shader::Shader&, shader::Shader&, bool>> programs) {
    std::vector<ShaderProgramCreation> results{};
    results.reserve(programs.size());

    for (auto& program : programs) {
        uint32_t program_id{0};
        if (!link_program(program_id, std::get<0>(program), std::get<1>(program))) {
            results.push_back(std::unexpected(GenericErr::Fail));
            continue;
        }

        results.push_back(ShaderProgram {
            .id = program_id,
            .using_standard_uniforms = std::get<2>(program)
        });
    };

    return results;
};

void use(uint32_t program) {
    glUseProgram(program);
};

}; // namespace shader_program