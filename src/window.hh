#pragma once

#include "glfw/glfw3.h"
#include <string>
#include <utility>
#include <functional>

struct Window {
    GLFWwindow* window_ptr{};
    int32_t width{800};
    int32_t height{600};

    operator GLFWwindow*() {
        return window_ptr;
    };
};

struct WindowParams {
    int32_t width{800};
    int32_t height{600};
    std::string name{"Cool window name :D"};
    int32_t gl_version_major{4};
    int32_t gl_version_minor{6};
};

Window create_window(const WindowParams& params);
bool is_window_valid(const Window& window);
void make_window_current(Window& window);
void set_window_resolution(Window& window, int32_t w, int32_t h);
void add_window_key_callback(Window& window, GLFWkeyfun cb);
void add_window_cursor_callback(Window& window, GLFWcursorposfun cb);
void add_window_framebuffer_size_callback(Window& window, GLFWframebuffersizefun cb);