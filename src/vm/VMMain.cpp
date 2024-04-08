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

#include <gui/Box.hpp>
#include <gui/Fonts.hpp>
#include <gui/Window.hpp>
#include <gui/vinogui.hpp>
#include "custom_errors.hpp"

#include <iostream>
#include <fstream>
#include <cstdint>
#include <filesystem>
#include <memory>

namespace vm_main {

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
    }

    InstructionsReader(const InstructionsReader &) = delete;
    InstructionsReader(InstructionsReader &&) = default;
    InstructionsReader &operator=(const InstructionsReader &) = delete;
    InstructionsReader &operator=(InstructionsReader &&) = delete;

    std::unique_ptr<IHandler> read_instruction();

private:
    std::ifstream _input_file{};

    char cur_byte{};

    vm_instr::Nop         nop{};
    vm_instr::Start       start{};
    vm_instr::Exit        exit{};
    vm_instr::LoadBg      loadbg{};
    vm_instr::ClearBg     clearbg{};
    vm_instr::LoadFg      loadfg{};
    vm_instr::LoadTxtLine loadtxtline{};
    vm_instr::Jmp         jmp{};
};

void main_loop(vino::Window &window)
{
    // vino::init_vinogui();
    fs::path m_vm_inst_path("./m_vm_inst.bin");
    if (!fs::exists(m_vm_inst_path)) {
        throw vino::VmError("No input m_vm_inst.bin");
    }

    InstructionsReader instr_reader(m_vm_inst_path);
    // read first instruction (must be START 0x1)
    // std::unique_ptr<IHandler> ptr_handler = instr_reader.read_instruction();
    // ptr_handler->handle_instruction();
    while (!window.should_close()) {
        window.update();
    }
}

enum class TitleStatus { exit, new_game, load_game, info };

TitleStatus title_screen(vino::Window &window)
{
    vino::FullscreenTexture title_screen(window, {0.0f, 0.0f, 0.0f, 1.0f});
    uint32_t                width_1_16 = window.get_width() / 16;
    uint32_t                height_1_9 = window.get_height() / 9;
    vino::SimpleBox bottom_up_rectangle({width_1_16 * 2, 0}, width_1_16 * 5,
            window.get_height(), window, {1.0f, 1.0f, 1.0f, 1.0f});
    vino::FontsCollection<char32_t> fonts;
    fonts.add_font_with_ascii(
            "./fonts/ARIALBD.ttf", static_cast<unsigned int>(width_1_16 * 0.5));

    vino::TextRenderer<char32_t> vino_title;
    vino::Button<char32_t>       but_new_game({width_1_16 * 3, height_1_9 * 5},
                  width_1_16 * 3, height_1_9, window, {1.0f, 1.0f, 1.0f, 1.0f},
                  U"New game", fonts["ARIALBD"], {}, {0.0f, 0.0f, 0.0f, 1.0f});

    vino::Button<char32_t> but_exit({width_1_16 * 3, height_1_9},
            width_1_16 * 3, height_1_9, window, {1.0f, 1.0f, 1.0f, 1.0f},
            U"Exit", fonts["ARIALBD"], {}, {0.0f, 0.0f, 0.0f, 1.0f});

    while (!window.should_close()) {
        if (but_exit.is_clicked()) {
            return TitleStatus::exit;
        }
        if (but_new_game.is_clicked()) {
            return TitleStatus::new_game;
        }

        title_screen.render();
        bottom_up_rectangle.render();
        vino_title.render_text(U"ViNo", fonts["ARIALBD"],
                {0.8f, 0.1f, 0.15f},
                {width_1_16 * 3, height_1_9 * 7}, window);
        but_new_game.render();
        but_exit.render();

        window.update();
    }
    return TitleStatus::exit;
}

}  // namespace vm_main

/// TODO: add arg --path
// int  check_args();
#ifdef _WIN32
int WinMain()
#else
int main()
#endif
{
    vino::NonResizableWindow main_window(1280, 720, "ViNo");
    main_window.set_icon("./vinovm.png");
    vm_main::TitleStatus status = vm_main::title_screen(main_window);
    if (status == vm_main::TitleStatus::new_game) {
        vm_main::main_loop(main_window);
    }
    return 0;
}

namespace vm_main {

/// TODO:Q: set exceptions flags for std::ifstream and remove all custom errors?
std::unique_ptr<IHandler> InstructionsReader::read_instruction()
{
    if (!_input_file.good()) {
        throw vino::VmError("Error:read_instruction(): Bad file");
    }
    _input_file.get(cur_byte);

    switch (cur_byte) {
        case 0x0:
            return std::make_unique<NopHandler>();

        case 0x1:
            if (!_input_file.read(reinterpret_cast<char *>(&start.start_pos),
                        sizeof(uint32_t)))
            {
                throw vino::VmError("Error:read_instruction(): Bad file");
            }
            if (_input_file.seekg(start.start_pos)) {
                throw vino::VmError(
                        "Error:read_instruction(): wrong starting position");
            }
            return std::make_unique<StartHandler>();

        case 0x2:
            if (_input_file.get(exit.err_code)) {
                throw vino::VmError("Error:read_instruction(): Bad file");
            }
            return std::make_unique<ExitHandler>();

        case 0x10:
            if (!_input_file.read(loadbg.path_bg.data(), 64)) {
                throw vino::VmError("Error:read_instruction(): Bad file");
            }
            return std::make_unique<LoadBgHandler>();

        case 0x11:
            return std::make_unique<ClearBgHandler>();

        case 0x20:
            if (!_input_file.read(loadfg.path_fg.data(), 64)) {
                throw vino::VmError("Error:read_instruction(): Bad file");
            }
            return std::make_unique<LoadFgHandler>();

        case 0x30:
            if (!_input_file.read(loadtxtline.txt_line.data(), 64)) {
                throw vino::VmError("Error:read_instruction(): Bad file");
            }
            return std::make_unique<LoadTxtLineHandler>();

        case 0x40:
            if (!_input_file.read(reinterpret_cast<char *>(&jmp.where_pos),
                        sizeof(uint32_t)))
            {
                throw vino::VmError("Error:read_instruction(): Bad file");
            }
            if (!_input_file.seekg(jmp.where_pos)) {
                throw vino::VmError(
                        "Error:read_instruction(): wrong JUMP instruction");
            }
            return std::make_unique<JmpHandler>();

        default:
            throw vino::VmError("Error: Wrong instruction");
    }
}

}  // namespace vm_main