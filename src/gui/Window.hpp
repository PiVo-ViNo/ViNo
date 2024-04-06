#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "custom_errors.hpp"

#include <cstdint>
#include <string>

namespace vino {

/**
 * @brief Base Window class
 * @warning Don't actually create glfw window
 */
class Window {
public:
    Window(const Window& other) = delete;

    virtual ~Window() { glfwTerminate(); }

    void make_current();
    void close();
    void swap_buffers();

    int get_attribute(int glfw_attribute);

    [[nodiscard]] bool should_close() const;
    [[nodiscard]] bool is_pressed(int glfw_key) const;
    [[nodiscard]] bool is_clicked() const;

    [[nodiscard]] std::pair<int, int> get_cursor_pos() const;
    [[nodiscard]] uint32_t            get_width() const;
    [[nodiscard]] uint32_t            get_height() const;

protected:
    Window(uint32_t width, uint32_t height);

    uint32_t    _width{};
    uint32_t    _height{};
    GLFWwindow* ptrWindow = nullptr;
};

class NonResizableWindow : public Window {
public:
    NonResizableWindow(
            uint32_t width, uint32_t height, const std::string& title);

    ~NonResizableWindow() override = default;

private:
};

// class ResizableWindow : public Window {};

}  // namespace vino