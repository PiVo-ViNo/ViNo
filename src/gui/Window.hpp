#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include "custom_errors.hpp"
#include <filesystem>
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

    virtual ~Window();

    void make_current();
    void close();
    void update(const glm::vec4& color = {0.0f, 0.0f, 0.0f, 1.0f});
    bool set_icon(const fs::path& path_to_icon);

    int get_attribute(int glfw_attribute);

    [[nodiscard]] bool should_close() const;
    [[nodiscard]] bool is_pressed(int glfw_key) const;
    [[nodiscard]] bool is_clicked() const;

    [[nodiscard]] std::pair<int, int> get_cursor_pos() const;
    [[nodiscard]] int            get_width() const;
    [[nodiscard]] int            get_height() const;

protected:
    Window(int width, int height);

    int _width{};
    int _height{};
    GLFWwindow* ptrWindow = nullptr;
};

class NonResizableWindow : public Window {
public:
    NonResizableWindow(const NonResizableWindow&) = delete;
    NonResizableWindow(NonResizableWindow&&) = delete;
    NonResizableWindow& operator=(const NonResizableWindow&) = delete;
    NonResizableWindow& operator=(NonResizableWindow&&) = delete;

    NonResizableWindow(
            int width, int height, const std::string& title);

    ~NonResizableWindow() override = default;

private:
};

// class ResizableWindow : public Window {};

}  // namespace vino