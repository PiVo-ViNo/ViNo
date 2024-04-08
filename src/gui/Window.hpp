#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "custom_errors.hpp"
#include <filesystem>
#include <cstdint>
#include <string>

namespace vino {

namespace fs = std::filesystem;

/**
 * @brief Base Window class
 * @warning Don't actually create glfw window
 */
class Window {
public:
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;
    Window(const Window& other) = delete;

    virtual ~Window() { glfwTerminate(); }

    void make_current();
    void close();
    void update(glm::vec4 color = {0.0f, 0.0f, 0.0f, 1.0f});
    bool set_icon(const fs::path& path_to_icon);

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
    NonResizableWindow(const NonResizableWindow&) = delete;
    NonResizableWindow(NonResizableWindow&&) = delete;
    NonResizableWindow& operator=(const NonResizableWindow&) = delete;
    NonResizableWindow& operator=(NonResizableWindow&&) = delete;

    NonResizableWindow(
            uint32_t width, uint32_t height, const std::string& title);

    ~NonResizableWindow() override = default;

private:
};

// class ResizableWindow : public Window {};

}  // namespace vino