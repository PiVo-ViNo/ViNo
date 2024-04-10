#include "Window.hpp"

#include "ImgData.hpp"
#include <cmath>
#include <filesystem>

namespace vino {

Window::Window(int width, int height) : _width(width), _height(height)
{
    assert(width > 0 && height > 0);
    if (glfwInit() == GLFW_FALSE) {
        throw vino::VmError("Error:Window(): cannot initialize GLFW library");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

Window::~Window() { glfwTerminate(); }

void Window::make_current()
{
    if (glfwGetCurrentContext() == ptrWindow) {
        return;
    }
    glfwMakeContextCurrent(ptrWindow);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(ptrWindow);
}

void Window::update(const glm::vec4& color)
{
    glfwSwapBuffers(ptrWindow);
    glfwPollEvents();
    glClearColor(color.r, color.g, color.b, color.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

int Window::get_attribute(int glfw_attribute)
{
    return glfwGetWindowAttrib(ptrWindow, glfw_attribute);
}

bool Window::is_pressed(int glfw_key) const
{
    return glfwGetKey(ptrWindow, glfw_key);
}

bool Window::is_clicked() const
{
    return glfwGetMouseButton(ptrWindow, GLFW_MOUSE_BUTTON_LEFT);
}

void Window::close()
{
    glfwSetWindowShouldClose(ptrWindow, GLFW_TRUE);
}

std::pair<int, int> Window::get_cursor_pos() const
{
    double xpos = 0;
    double ypos = 0;
    glfwGetCursorPos(ptrWindow, &xpos, &ypos);
    return {std::floor(xpos), _height - std::floor(ypos)};
}

int Window::get_width() const
{
    return _width;
}

int Window::get_height() const
{
    return _height;
}

bool Window::set_icon(const fs::path& path_to_icon)
{
    if (!fs::exists(path_to_icon)) {
        return false;
    }
    ImgData icon_data(path_to_icon.string(), false);
    GLFWimage icon = {icon_data.width, icon_data.height, icon_data.data};
    glfwSetWindowIcon(ptrWindow, 1, &icon);
    return true;
}

NonResizableWindow::NonResizableWindow(
        int width, int height, const std::string& title) :
    Window(width, height)
{
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    ptrWindow =
            glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr);
    if (ptrWindow == nullptr) {
        glfwTerminate();
        throw vino::WindowError("Window wasn't successfully initialized");
    }
    make_current();
}

}  // namespace vino