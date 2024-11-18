#include "window.hh"

Window create_window(const WindowParams& params) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, params.gl_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, params.gl_version_minor);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(params.width, params.height,
                                          params.name.c_str(), NULL, NULL);
    if (window == NULL) {
        glfwTerminate();
        return Window{.window_ptr = nullptr};
    }

    return Window{
        .window_ptr = window,
        .width = params.width,
        .height = params.height
    };
}

bool is_window_valid(const Window& window) {
    return window.window_ptr != nullptr;
};

void make_window_current(Window& window) {
    glfwMakeContextCurrent(window);
};

void set_window_resolution(Window& window, int32_t w, int32_t h) {
    glfwSetWindowSize(window, w, h);
    window.width = w;
    window.height = h;
};

void add_window_key_callback(Window& window, GLFWkeyfun cb) {
    glfwSetKeyCallback(window, cb);
};

void add_window_cursor_callback(Window& window, GLFWcursorposfun cb) {
    glfwSetCursorPosCallback(window, cb);
};

void add_window_framebuffer_size_callback(Window& window, GLFWframebuffersizefun cb) {
    glfwSetFramebufferSizeCallback(window, cb);
};