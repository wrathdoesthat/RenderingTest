#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/TextEditor.h"

#include "glfw/glfw3.h"

struct UIData {
    bool show_ui{true};
    char shader_path_buffer[64];
    bool change_texture{false};
    float texture_bias{0.5};
    bool show_editor{true};
    float mesh_scale{1};
    bool do_postprocess{false};
};

void prepare_imgui(GLFWwindow* window);
void render_ui(GLFWwindow* window, UIData& ui_data);
void setup_ui_style();