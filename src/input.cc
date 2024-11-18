#include "input.hh"

InputState& get_input_state() {
    static InputState input_state;
    return input_state;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action,
                  int mods) {
    InputState& input_state = get_input_state();
    if (key == GLFW_KEY_INSERT && action == GLFW_RELEASE) {
        input_state.capture_mouse = !input_state.capture_mouse;
    }
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    InputState& input_state = get_input_state();
    if (input_state.first_mouse) {
        input_state.last_x = xpos;
        input_state.last_y = ypos;
        input_state.first_mouse = false;
    }

    if (input_state.capture_mouse == false) {
        return;
    }

    input_state.x_offset = xpos - input_state.last_x;
    input_state.y_offset = input_state.last_y - ypos;
    input_state.last_x = xpos;
    input_state.last_y = ypos;
}