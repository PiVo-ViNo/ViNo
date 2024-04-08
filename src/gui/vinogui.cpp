#include "vinogui.hpp"

static void vino::error_callback(int error, const char* description)
{
    std::cerr << "Error:GLFW:" << error << ": " << description << std::endl;
    throw vino::WindowError(description);
}

void vino::init_vinogui()
{
    glfwSetErrorCallback(error_callback);
}