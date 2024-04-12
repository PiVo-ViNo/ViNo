/*
 * VMMain.cpp
 *
 * Created on Wed Apr 03 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#include "VMAtoms.hpp"
#include "VMHandler.hpp"

#include <gui/vinogui.hpp>
#include <custom_errors.hpp>
#include <glm.hpp>

#include <chrono>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <memory>
#include <string>

namespace vino_vm {

namespace fs = std::filesystem;

class InstructionsReader {
public:
    explicit InstructionsReader(
            const fs::path &input_file, char start_byte = 0) :
        _input_file(input_file, std::ios::in | std::ios::binary),
        cur_byte(std::move(start_byte))
    {
        if (!_input_file.is_open()) {
            throw vino::VmError(
                    "Error:InstructionsReader(): cannot open input file");
        }
        using ifs = std::ifstream;
        _input_file.exceptions(ifs::failbit | ifs::eofbit | ifs::badbit);
    }

    InstructionsReader(const InstructionsReader &) = delete;
    InstructionsReader(InstructionsReader &&) = default;
    InstructionsReader &operator=(const InstructionsReader &) = delete;
    InstructionsReader &operator=(InstructionsReader &&) = delete;

    std::unique_ptr<IHandler> read_instruction();

private:
    std::ifstream _input_file{};

    char cur_byte{};

    // vm_instr::Nop         nop{};
    vm_instr::Start  start{};
    vm_instr::Exit   exit{};
    vm_instr::LoadBg loadbg{};
    // vm_instr::ClearBg     clearbg{};
    vm_instr::LoadFg      loadfg{};
    vm_instr::LoadTxtLine loadtxtline{};
    // vm_instr::TxtLineBreakage breakage{};
    vm_instr::Jmp jmp{};
};

void open_screen(vino::Window &window)
{
    vino::ImgData img("res/title_screen.png");

    vino::FullscreenTexture title_tex(window, img);

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

GameStatus main_menu_screen(vino::Window &window)
{
    vino::FullscreenTexture title_screen(
            window, vino::ImgData("res/jack.png"));
    int width_1_16 = window.get_width() / 16;
    int height_1_9 = window.get_height() / 9;

    vino::FontsCollection<char32_t> fonts;
    fonts.add_font_with_ascii(
            "./fonts/ARIALBD.ttf", static_cast<int>(width_1_16 * 0.5));
    fonts.add_font_with_ascii(
            "./fonts/ARIAL.ttf", static_cast<int>(width_1_16 * 0.25));

    vino::SimpleBox bottom_up_rectangle({width_1_16 * 2, 0}, width_1_16 * 5,
            window.get_height(), window, {1.0f, 1.0f, 1.0f, 0.5f});

    vino::TextRenderer<char32_t> vino_title;
    vino::SimpleBox vino_letter({width_1_16 * 13, height_1_9 * 0.2},
            static_cast<int>(width_1_16 * 0.5),
            static_cast<int>(height_1_9 * 0.5), window,
            vino::ImgData("vinovm.png"));

    vino::Button<char32_t> but_new_game({width_1_16 * 3, height_1_9 * 5},
            width_1_16 * 3, height_1_9, window, {1.0f, 1.0f, 1.0f, 1.0f},
            U"New game", fonts["ARIALBD"], {}, {0.0f, 0.0f, 0.0f, 1.0f});

    vino::Button<char32_t> but_exit({width_1_16 * 3, height_1_9},
            width_1_16 * 3, height_1_9, window, {1.0f, 1.0f, 1.0f, 1.0f},
            U"Exit", fonts["ARIALBD"], {}, {0.0f, 0.0f, 0.0f, 1.0f});

    while (!window.should_close()) {
        if (but_exit.is_clicked()) {
            return GameStatus::close_app;
        }
        if (but_new_game.is_clicked()) {
            return GameStatus::new_game;
        }

        title_screen.render();
        bottom_up_rectangle.render();
        vino_letter.render();
        /// TODO: function box.clip_to_render(IBox&) to simplify rendering many
        /// elements
        vino_title.render_text(U"iNo version 0.1a", fonts["ARIAL"],
                {0.9f, 0.9f, 0.9f},
                {static_cast<unsigned int>(vino_letter.get_low_left_pos().x)
                                + vino_letter.get_width() * 0.9,
                        height_1_9 * 0.27},
                window);
        but_new_game.render();
        but_exit.render();

        window.update();
    }
    return GameStatus::close_app;
}

GameStatus main_loop(vino::Window &window)
{
    vino::init_vinogui();

    fs::path m_vm_inst_path("./m_vm_inst_test.bin");
    if (!fs::exists(m_vm_inst_path)) {
        throw vino::VmError("No input m_vm_inst.bin");
    }

    vino::FontsCollection<char32_t> fonts;
    fonts.add_font_with_ascii("fonts/ARIAL.ttf", 30);

    GuiInterface main_gui(&window);
    main_gui.fullscreen_textures.emplace_back(
            window, vino::ImgData("res/jack.png"));
    main_gui.low_boxes.emplace_back(window, glm::ivec2{10, 10},
            glm::uvec2{window.get_width() - 20, window.get_height() / 3},
            glm::vec4{0.8, 0.7, 0.7, 0.8}, glm::vec4{0.7, 0.7, 0.7, 0.9},
            fonts["ARIAL"]);

    InstructionsReader instr_reader(m_vm_inst_path);
    // read first instruction (must be START 0x1, but no checks since it's VM)
    std::unique_ptr<IHandler> ptr_handler = instr_reader.read_instruction();
    bool continue_handling = ptr_handler->handle_instruction(main_gui);

    using dur = std::chrono::duration<float>;
    using sys_time = std::chrono::system_clock;
    auto cur_time = sys_time::now();
    auto dot_time_counter = sys_time::now();

    while (main_gui.exit_flag == GameStatus::not_set && !window.should_close())
    {
        if (dur(sys_time::now() - dot_time_counter).count() >= 0.2f) {
            main_gui.low_boxes.front().add_text(U".");
            dot_time_counter = sys_time::now();
        }
        if (window.is_pressed(GLFW_KEY_LEFT_CONTROL)
                || (window.is_pressed(GLFW_KEY_SPACE)
                        && dur(sys_time::now() - cur_time).count() >= 0.5f))
        {
            if (!main_gui.low_boxes.back().next_slide()) {
                do {
                    continue_handling = instr_reader
                                        .read_instruction()
                                        ->handle_instruction(main_gui);
                } while (continue_handling);
            }
            cur_time = sys_time::now();
        }

        main_gui.smart_render();

        window.update();
    }

    if (window.should_close()) {
        main_gui.exit_flag = GameStatus::close_app;
    }
    return main_gui.exit_flag;
}

}  // namespace vino_vm

/// TODO: add arg --path
// int  check_args();
#ifdef _WIN32
int WinMain()
#else
int main()
#endif
{
    auto start = std::chrono::system_clock::now();
    try {
        vino::init_vinogui();
        vino::NonResizableWindow main_window(1280, 720, "ViNo");
        main_window.set_icon("./vinovm.png");
        vino_vm::GuiInterface main_gui(&main_window);
        vino_vm::open_screen(main_window);
        vino_vm::GameStatus status = vino_vm::GameStatus::main_menu;
        while (status != vino_vm::GameStatus::close_app) {
            switch (status) {
                case vino_vm::GameStatus::close_app:
                    break;
                case vino_vm::GameStatus::main_menu:
                    status = vino_vm::main_menu_screen(main_window);
                    break;
                case vino_vm::GameStatus::new_game:
                    status = vino_vm::main_loop(main_window);
                    break;
                case vino_vm::GameStatus::load_game:
                    // [WIP] ... load game screen with saves...
                    break;
                case vino_vm::GameStatus::info:
                    // [WIP] ... info screen ...
                    break;
                case vino_vm::GameStatus::not_set:
                    // ???
                    break;
            }
        }
        main_window.close();
    } catch (std::exception &exc) {
        float time = std::chrono::duration<float>(
                std::chrono::system_clock::now() - start)
                             .count();
        vino::NonResizableWindow  err_window(500, 200, "ViNo Error");
        vino::FullscreenTexture   err_bg(err_window, {1.0f, 1.0f, 1.0f, 1.0f});
        vino::StaticTextBox<char> err_box(
                {50, 0}, 450, 185, err_window, {1.0f, 1.0f, 1.0f, 1.0f});
        vino::SimpleBox err_symbol({10, 150}, 30, 30, err_window,
                vino::ImgData("error_symbol.png"));

        vino::FontsCollection<char> fonts;
        /// TODO: make possible to choose different sizes
        fonts.add_font_with_ascii("fonts/ARIAL.ttf", 22);

        while (!err_window.should_close()) {
            err_bg.render();
            err_symbol.render();
            err_box.render_text("time: " + std::to_string(time), fonts["ARIAL"],
                    {0.0f, 0.0f, 0.0f, 1.0f});

            err_window.update();
        }
    }
    return 0;
}

namespace vino_vm {

/// TODO:Q: set exceptions flags for std::ifstream and remove all custom errors?
std::unique_ptr<IHandler> InstructionsReader::read_instruction()
{
    if (!_input_file.good()) {
        return std::make_unique<ExitHandler>(3);
    }
    try {
        _input_file.get(cur_byte);

        switch (cur_byte) {
            case 0x0:
                return std::make_unique<NopHandler>();

            case 0x1:
                _input_file.read(reinterpret_cast<char *>(&start.start_pos),
                        sizeof(uint32_t));
                _input_file.seekg(start.start_pos);
                return std::make_unique<StartHandler>(start.start_pos);

            case 0x2:
                try {
                    _input_file.get(exit.err_code);
                } catch (const std::ifstream::failure &err) {
                    if (_input_file.eof()) {
                        exit.err_code = 0;
                    } else {
                        throw err;
                    }
                }
                return std::make_unique<ExitHandler>(exit.err_code);

            case 0x10:
                _input_file.read(loadbg.path_bg.data(), 64);
                return std::make_unique<LoadBgHandler>(loadbg);

            case 0x11:
                return std::make_unique<ClearBgHandler>();

            case 0x20:
                _input_file.read(loadfg.path_fg.data(), 64);
                return std::make_unique<LoadFgHandler>(loadfg);

            case 0x30:
                _input_file.read(loadtxtline.txt_line.data(), 64);
                return std::make_unique<LoadTxtLineHandler>(loadtxtline);

            case 0x31:
                return std::make_unique<PreBreakageHandler>();
            
            case 0x32:
                return std::make_unique<TxtLineBreakageHandler>();

            case 0x40:
                _input_file.read(reinterpret_cast<char *>(&jmp.where_pos),
                        sizeof(uint32_t));
                _input_file.seekg(jmp.where_pos);
                return std::make_unique<JmpHandler>();

            default:
                return std::make_unique<NopHandler>();
                // throw vino::VmError("Error: Wrong instruction");
        }
    } catch (const std::ifstream::failure &failure) {
        std::cout << failure.what() << std::endl;
        exit.err_code = 2;
        return std::make_unique<ExitHandler>(2);
    }
}

}  // namespace vino_vm