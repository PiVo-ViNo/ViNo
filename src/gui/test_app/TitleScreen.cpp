#include "TitleScreen.hpp"

#include "Box.hpp"
#include "ImgData.hpp"
#include <GLFW/glfw3.h>

namespace vino {

void titleScreen(vino::Window& window)
{
    // load .png, generate it
    ImgData img("res/title_screen.png");

    FullscreenTexture title_tex(window, img);

    glfwSetTime(0.0);

    while (!window.should_close()) {
        double time = glfwGetTime();
        if (time > 6.0 || window.is_pressed(GLFW_KEY_SPACE)) {
            return;
        }
        float alpha = -((time - 2.5) / 2.5) * ((time - 2.5) / 2.5) + 1;

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        title_tex.render(alpha);

        window.swap_buffers();
        glfwPollEvents();
    }
}

}  // namespace vino