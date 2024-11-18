#include "camera.hh"

glm::mat4x4 update_camera(GLFWwindow* window, Camera& camera, const float dt) {
    InputState& input_state = get_input_state();

    float sensitivity = 0.1f;
    input_state.x_offset *= sensitivity;
    input_state.y_offset *= sensitivity;

    camera.yaw += input_state.x_offset;
    camera.pitch += input_state.y_offset;

    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    glm::vec3 direction;
    direction.x =
        cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    direction.y = sin(glm::radians(camera.pitch));
    direction.z =
        sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    camera.front = glm::normalize(direction);

    camera.right = glm::normalize(glm::cross(camera.front, glm::vec3(0, 1, 0)));
    camera.up = glm::normalize(glm::cross(camera.right, camera.front));

    float cameraSpeed = 2.5 * dt;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.position += cameraSpeed * camera.front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.position -= cameraSpeed * camera.front;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.position -=
            glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.position +=
            glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    }

    return glm::lookAt(camera.position, camera.position + camera.front,
                       camera.up);
};