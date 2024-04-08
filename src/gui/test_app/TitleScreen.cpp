#include "TitleScreen.hpp"

#include <chrono>

namespace vino {

void titleScreen(vino::Window& window)
{
    // load .png, generate it
    ImgData img("res/title_screen.png");

    FullscreenTexture title_tex(window, img);

    namespace sch = std::chrono;
    auto start = sch::system_clock::now();

    while (!window.should_close()) {
        float time =
                std::chrono::duration<float>(sch::system_clock::now() - start)
                        .count();
        if (time > 6.0f || window.is_pressed(GLFW_KEY_SPACE)) {
            return;
        }
        float alpha = -((time - 2.5f) / 2.5f) * ((time - 2.5f) / 2.5f) + 1;
        title_tex.render(alpha);

        window.update({0.0f, 0.0f, 0.0f, 1.0f});
    }
}

}  // namespace vino