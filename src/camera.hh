#pragma once

#include "input.hh"

#include "GLFW/glfw3.h"

#include <glm/glm.hpp>

struct Camera {
    glm::vec3 position{0, 0, 3.0f};
    glm::vec3 front{0, 0, 0};
    glm::vec3 up{0, 0, 0};
    glm::vec3 right{0, 0, 0};

    // Input stuff
    bool first_mouse{true};
    double last_x{0};
    double last_y{0};
    double yaw{0};
    double pitch{0};
};

glm::mat4x4 update_camera(GLFWwindow* window, Camera& camera, const float dt);