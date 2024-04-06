#include "Window.hpp"
#include <GLFW/glfw3.h>
#include <cmath>

namespace vino {

Window::Window(uint32_t width, uint32_t height) : _width(width), _height(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

void Window::make_current()
{
    glfwMakeContextCurrent(ptrWindow);
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(ptrWindow);
}

void Window::swap_buffers()
{
    glfwSwapBuffers(ptrWindow);
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
    return {std::floor(xpos), 600 - std::floor(ypos)};
}

uint32_t Window::get_width() const
{
    return _width;
}

uint32_t Window::get_height() const
{
    return _height;
}

NonResizableWindow::NonResizableWindow(
        uint32_t width, uint32_t height, const std::string& title) :
    Window(width, height)
{
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    ptrWindow =
            glfwCreateWindow(_width, _height, title.c_str(), nullptr, nullptr);
    if (ptrWindow == nullptr) {
        glfwTerminate();
        throw vino::WindowError("Window wasn't successfully initialized");
    }
}

}  // namespace vino