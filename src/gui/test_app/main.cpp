#include "Window.hpp"
#include "MainMenu.hpp"
#include "TitleScreen.hpp"
#include "Box.hpp"
#include "custom_errors.hpp"
#include "GLFW/glfw3.h"

#include <iostream>
#include <stdexcept>
#include <filesystem>

#ifdef _WIN32
int WinMain()
#else
int main()
#endif
{
    // std::cout << std::filesystem::current_path() << std::endl;
    vino::NonResizableWindow main_window(800, 600, "ViNo");
    main_window.make_current();

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw vino::WindowError("Failed to initialize GLAD");
    }

    try {
        // show title ViNo for 7s
        vino::titleScreen(main_window);

        // show main menu
        vino::mainMenu(main_window);

        throw std::runtime_error("ОЛЕГУС BOM BOM BOM!");
    } catch (std::exception& ex) {
        // std::cout << ex.what() << std::endl;
        vino::NonResizableWindow err_window(500, 200, "ViNo Error");
        err_window.make_current();

        vino::StaticTextBox<char> err_box({0, 0}, 500, 200, err_window,
                                          {1.0f, 1.0f, 1.0f, 1.0f});

        vino::FontsCollection<char> fonts;
        /// TODO: make possible to choose different sizes
        fonts.add_font_with_ascii("fonts/ARIAL.ttf", 22);
        while (!err_window.should_close()) {
            if (err_window.is_pressed(GLFW_KEY_ESCAPE)) {
                err_window.close();
            }

            err_box.render();
            err_box.render_text("Error: " + std::string(ex.what()),
                                fonts["ARIAL"], {0.0f, 0.0f, 0.0f, 1.0f});
            err_window.swap_buffers();
            glfwPollEvents();
        }
    }

    return 0;
}
