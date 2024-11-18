#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#undef GLFW_INCLUDE_NONE

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Windows.h>

#include "camera.hh"
#include "glad/glad.h"
#include "input.hh"
#include "shader.hh"
#include "shader_program.hh"
#include "texture.hh"
#include "ui.hh"
#include "utils.hh"
#include "window.hh"

/* #include "spdlog/fmt/ostr.h"
#include "spdlog/spdlog.h" */

#include <chrono>
#include <format>
#include <iostream>

using namespace std::chrono;
using namespace std::chrono_literals;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // spdlog::info(std::format("Window resized {} {}", width, height));
    glViewport(0, 0, width, height);
};

UIData ui_data;
bool do_postprocess{false};

int main() {
    time_point program_start_time = high_resolution_clock::now();

    // TODO: why does ShowWindow on the console handle break everything
    // WHy is this the only way i can get the console window to show
    // Please :(
    FreeConsole();
    AllocConsole();

    Window window = create_window(WindowParams{});
    if (!is_window_valid(window)) {
        std::cout << "Creating window failed\n";
        return 1;
    }

    make_window_current(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 1;
    }

    add_window_cursor_callback(window, mouse_callback);
    add_window_key_callback(window, key_callback);
    add_window_framebuffer_size_callback(window, framebuffer_size_callback);

    glViewport(0, 0, 800, 600);

    float fullscreen_quad_vertices[] = {
        -1.0, 1.0,  0.0, // L TL
        -1.0, -1.0, 0.0, // LR BL
        1.0,  1.0,  0.0, // LR TR
        1.0,  -1.0, 0.0  // R BL
    };

    float colored_triangle_data[] = {
        -1.0f, -1.0f, 0.0f, 1.0, 0.0, 0.0, // BL
        0.0f,  1.0f,  0.0f, 0.0, 1.0, 0.0, // M
        1.0f,  -1.0f, 0.0f, 0.0, 0.0, 1.0  // BR
    };

    float vertices[] = {
        // positions // colors // texture coords
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
        0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };

    float cube_vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,
        0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, 0.5f,  0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f,
        -0.5f, 0.5f,  -0.5f, 0.0f,  0.0f,  -1.0f, -0.5f, -0.5f, -0.5f, 0.0f,  0.0f,  -1.0f,

        -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, 0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  -0.5f, -0.5f, 0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,  -0.5f, -0.5f, -0.5f, -1.0f, 0.0f,  0.0f,
        -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f,  0.0f,  -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 1.0f,  0.0f,  0.0f,
        0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,  0.5f,  -0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
        0.5f,  -0.5f, 0.5f,  1.0f,  0.0f,  0.0f,  0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,
        0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  0.5f,  -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f,  0.0f,  -1.0f, 0.0f,  -0.5f, -0.5f, -0.5f, 0.0f,  -1.0f, 0.0f,

        -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  -0.5f, 0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f, 0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  -0.5f, 0.5f,  -0.5f, 0.0f,  1.0f,  0.0f};

    float vertices2[] = {
        // positions // texture coords
        1.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top right
        1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -1.0f, 1.0f,  0.0f, 0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    stbi_set_flip_vertically_on_load(true);

    Texture base_texture;
    create_texture("Textures/trans.png", base_texture);

    Texture overlay_texture;
    create_texture("Textures/hawg.png", overlay_texture);

    unsigned int framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    unsigned int textureColorbuffer;
    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer,
                           0);

    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // texture coordinates
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Light cube
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    unsigned int lbvo;
    glGenBuffers(1, &lbvo);
    glBindBuffer(GL_ARRAY_BUFFER, lbvo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Screenspace quad
    unsigned int VAO2;
    glGenVertexArrays(1, &VAO2);
    glBindVertexArray(VAO2);

    unsigned int VBO2;
    glGenBuffers(1, &VBO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

    // positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // texture coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int EBO2;
    glGenBuffers(1, &EBO2);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    std::vector<shader::ShaderCreation> shdr_creation_statuses = shader::create_bulk({
        {"Shaders/lighting_vertex_shader.vs", GL_VERTEX_SHADER},
        {"Shaders/lighting_fragment_shader.fs", GL_FRAGMENT_SHADER},
        {"Shaders/vertex_shader2.vs", GL_VERTEX_SHADER},
        {"Shaders/light_cube_shader.fs", GL_FRAGMENT_SHADER},
        {"Shaders/vertex_shader.vs", GL_VERTEX_SHADER},
        {"Shaders/postprocess_fragment_shader.fs", GL_FRAGMENT_SHADER},
    });

    if (!is_expected_values(shdr_creation_statuses)) {
        std::cout << "Start failed shaders failed\n";
        return 1;
    };

    std::vector<shader::Shader> shaders = get_expected_values(shdr_creation_statuses);
    std::vector<shader_program::ShaderProgramCreation> prgrm_creation_status =
        shader_program::create_bulk({{shaders[0], shaders[1], true},
                                     {shaders[2], shaders[3], true},
                                     {shaders[4], shaders[5], true}});

    if (!is_expected_values(prgrm_creation_status)) {
        std::cout << "Shader program links failed\n";
        return 1;
    }

    std::vector<shader_program::ShaderProgram> programs =
        get_expected_values(prgrm_creation_status);

    auto program = programs[0];
    auto light_cube_program = programs[1];
    auto postprocess_program = programs[2];

    prepare_imgui(window);
    setup_ui_style();

    float delta_time{0};
    float last_frame{0};

    Camera camera;

    glm::vec3 lightPos(1, 1, 1);

    glm::vec3 object_color(1.0, 1.0, 1.0);
    glm::vec3 light_color(1, 1, 1);

    struct cube_data {
        glm::vec3 translation{0.0, 0.0, 0.0};
        glm::vec3 color{0.0, 0.0, 0.0};
        float rotationdeg{0};
        float scale{0};
    };

    std::vector<cube_data> cube_vec{
        cube_data{
            .translation{3, 0, 0},
            .color{0.2, 0.4, 0.7},
            .rotationdeg{90},
            .scale{1.5},
        },
    };

    while (!glfwWindowShouldClose(window)) {
        float current_frame = glfwGetTime();
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        namespace sp = shader_program;

        InputState& input_state = get_input_state();
        glfwSetInputMode(window, GLFW_CURSOR,
                         input_state.capture_mouse ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);

        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glClearColor(0.2, 0.1, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        sp::use(program);
        sp::set_uniform(program, "light_color", light_color);
        sp::set_uniform(program, "light_position", lightPos);

        glBindVertexArray(VAO);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        glm::mat4 view = update_camera(window, camera, delta_time);

        sp::set_uniform(program, "projection", projection);
        sp::set_uniform(program, "view", view);

        for (const cube_data& cube : cube_vec) {
            sp::set_uniform(program, "object_color", cube.color);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube.translation);
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(cube.rotationdeg),
                                glm::vec3(0.5f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(cube.scale));

            sp::set_uniform(program, "model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        sp::use(light_cube_program);
        sp::set_uniform(light_cube_program, "light_color", light_color);
        sp::set_uniform(light_cube_program, "projection", projection);
        sp::set_uniform(light_cube_program, "view", view);

        glm::mat4 light_model = glm::mat4(1.0f);
        light_model = glm::translate(light_model, lightPos);
        light_model = glm::scale(light_model, glm::vec3(1));
        sp::set_uniform(light_cube_program, "model", light_model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Postprocess stuff
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.2, 0.1, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

        sp::use(postprocess_program);
        sp::set_uniform(postprocess_program, "u_tex", 0);
        sp::set_uniform(postprocess_program, "u_enable", ui_data.do_postprocess);

        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        render_ui(window, ui_data);

        if (ui_data.change_texture) {
            create_texture(ui_data.shader_path_buffer, overlay_texture);
        }

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
};