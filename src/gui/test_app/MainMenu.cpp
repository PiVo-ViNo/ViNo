#include "MainMenu.hpp"

#include "Box.hpp"
#include "Fonts.hpp"
#include "ImgData.hpp"
#include "Shader.hpp"

#include <GLFW/glfw3.h>
#include <deque>
#include <string>

namespace vino {

void mainMenu(Window& window)
{
    Shader menu_shader("shaders/basicVertex.glsl",
                       "shaders/basicFrag.glsl");
    menu_shader.use();
    menu_shader.setInt("uTexture", 0);

    ImgData           fs_img("res/olegus.png");
    FullscreenTexture fs_texture(window, fs_img);

    FontsCollection<char32_t> fonts;
    /// TODO: make possible to choose different sizes
    fonts.add_font_with_ascii("fonts/ARIALBI.ttf",
                              22 * window.get_width() / 800);
    fonts.add_font_with_ascii("fonts/ARIALBD.ttf",
                              22 * window.get_width() / 800);
    // Font   arial = fonts["ARIAL"];
    Button<char32_t> exit_button({10, window.get_height() - 60}, 100, 50,
                                 window, {1.0f, 1.0f, 1.0f, 0.8f}, U"Exit",
                                 fonts["ARIALBD"], {}, {0.2, 0.2, 0.2, 0.8});
    LowBox<char32_t> low_box(window, {10, 10},
                             {window.get_width() - 20, window.get_height() / 3},
                             fonts["ARIALBI"]);

    ImgData          rin("res/rin.png");
    ForegroundFigure olegus({100, 50}, 300, 500, window, rin);

    std::deque<std::u32string> texts;

    texts.emplace_back(
        U"Теперь на самом деле даже толку нет бить глебуса молотком по "
        U"голове.");
    texts.emplace_back(
        U"A church.... To-day, no incense to \n"
        "Its round dome coils, nor do a prayer \n"
        "The humble monks chant, hoarse-voiced, there. \n"
        "Alone, forgot by death and men, \n"
        "A bent old greybeard, denizen \n"
        "Of these remote and desolate hills, \n"
        "Over the ruins watches still ");
    texts.emplace_back(
        U"And daily wipes the dust that clings \n"
        "To tombs, of which the letterings \n"
        "Of glories past speak and of things \n"
        "Of like note. Of a tsar one such \n"
        "Tells who by his gold crown was much \n"
        "Weighed down, and did of Russia gain \n"
        "The patronage o'er his domain. \n"
        "Twas then God's love descended on \n"
        "The land, and Georgia bloomed, and gone \n"
        "Her old fears were and old suspense: \n"
        "Of friendly bayonets a fence \n"
        "Did, bristling, rise in her defence.");

    std::size_t cur_text = 0;

    /// TODO: Don't use glfw time, it could be zeroed anywhere, use std::chrono
    glfwSetTime(0);

    while (!window.should_close()) {
        if (exit_button.is_clicked()) {
            window.close();
        }
        if (glfwGetTime() >= 0.5 && window.is_pressed(GLFW_KEY_SPACE)) {
            cur_text = (cur_text + 1) % texts.size();
            glfwSetTime(0);
            olegus.move_with_clip({-50, 0});
        }

        glClearColor(0.2f, 0.7f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ///Q: Must auto sort rendering be made?
        fs_texture.render();
        olegus.render();
        low_box.render(U"Олегус", texts[cur_text]);
        exit_button.render();

        window.swap_buffers();
        glfwPollEvents();
    }
}

}  // namespace vino