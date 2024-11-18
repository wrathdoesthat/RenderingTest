#pragma once

#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct InputState {
    bool first_mouse{true};
    float last_x{0};
    float last_y{0};
    float x_offset{0};
    float y_offset{0};
    bool capture_mouse{false};
};

// lol
InputState& get_input_state();
void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mods);
void do_input(GLFWwindow* window, const float dt);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);