/*
 * VMHandler.hpp
 *
 * Created on Sun Apr 07 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#pragma once
#include "vm/VMAtoms.hpp"

#include <gui/vinogui.hpp>
#include <stuff.hpp>
#include <custom_errors.hpp>

#include <string>
#include <vector>

namespace vino_vm {

enum class GameStatus {
    not_set,
    close_app,
    main_menu,
    new_game,
    load_game,
    info
};

/// TODO:Q: Enclose this struct (make it true class)?
struct GuiInterface {
    explicit GuiInterface(vino::Window* window) : parent_window(window) {}

    // void render_front()
    // {
    //     if (!fullscreen_textures.empty()) {
    //         fullscreen_textures.front().render();
    //     }
    //     if (!stat_text_boxes.empty()) {
    //         stat_text_boxes.front().render();
    //     }
    //     if (!buttons.empty()) {
    //         buttons.render
    //     }
    // }

    void smart_render()
    {
        if (!fullscreen_textures.empty()) {
            fullscreen_textures.back().render();
        }
        if (!stat_text_boxes.empty()) {
            stat_text_boxes.back().render();
        }
        for (const auto& fg : fg_figures) {
            fg.render();
        }
        for (const auto& box : simple_boxes) {
            box.render();
        }
        if (!low_boxes.empty()) {
            low_boxes.back().render();
        }
        for (const auto& box : stat_text_boxes) {
            box.render();
        }
        for (const auto& button : buttons) {
            button.render();
        }
    }

    std::vector<vino::FullscreenTexture>       fullscreen_textures{};
    std::vector<vino::ForegroundFigure>        fg_figures{};
    std::vector<vino::SimpleBox>               simple_boxes{};
    std::vector<vino::LowBox<char32_t>>        low_boxes{};
    std::vector<vino::StaticTextBox<char32_t>> stat_text_boxes{};
    std::vector<vino::Button<char32_t>>        buttons{};

    vino::Window* parent_window;
    GameStatus    exit_flag{GameStatus::not_set};
};

class IHandler {
public:
    virtual ~IHandler() = default;
    /**
     * @brief Handle next instruction.
     *
     * @param[in,out] gui Reference to GUI interface.
     * @retval true   If a next instruction must be handled 
     * @retval false  Stop handling 
     */
    virtual bool handle_instruction(GuiInterface&) = 0;
};

class NopHandler : public IHandler {
public:
    bool handle_instruction(GuiInterface& gui) override
    {
        gui.low_boxes.back().update_text(U"NOP");
        return true;
    }
};

class StartHandler : public IHandler {
public:
    explicit StartHandler(uint32_t start_pos) : _start(start_pos) {}

    bool handle_instruction(GuiInterface& gui) override
    {
        // ... init values, etc.
        gui.low_boxes.back().update_text(U"START");
        return true;
    }

private:
    vm_instr::Start _start{};
};

class ExitHandler : public IHandler {
public:
    explicit ExitHandler(char exit_code) : exit(exit_code) {}

    bool handle_instruction(GuiInterface& gui) override
    {
        // Error code:  0 - no errors, close application
        //              1 - no errors, exit to main menu
        //              2 - bad instructions file
        switch (exit.err_code) {
            case 0:
                gui.exit_flag = GameStatus::close_app;
                break;
            case 1:
                gui.exit_flag = GameStatus::main_menu;
                break;
            default:
                throw vino::VmError("Exit instruction error: Bad file");
        }
        return false;
    }

private:
    vm_instr::Exit exit{};
};

class LoadBgHandler : public IHandler {
public:
    explicit LoadBgHandler(const vm_instr::LoadBg& loadbg) : _loadbg(loadbg) {}

    bool handle_instruction(GuiInterface& gui) override
    {
        if (gui.fullscreen_textures.empty()) {
            gui.fullscreen_textures.emplace_back(*gui.parent_window,
                    vino::ImgData(
                            "res/" + std::string(_loadbg.path_bg.data())));
        } else {
            gui.fullscreen_textures.back().change_texture(vino::ImgData(
                    "res/" + std::string(_loadbg.path_bg.data())));
        }
        gui.low_boxes.back().update_text(
                U"LOADBG: "
                + vino::convert_str<char, char32_t>(_loadbg.path_bg.data()));
        return false;
    }

private:
    vm_instr::LoadBg _loadbg{};
};

class ClearBgHandler : public IHandler {
public:
    bool handle_instruction(GuiInterface& gui) override
    {
        /// TODO: Make it more cache friendly
        gui.fg_figures.clear();
        return false;
    }

private:
    // vm_instr::ClearBg clearbg;
};

class LoadFgHandler : public IHandler {
public:
    explicit LoadFgHandler(const vm_instr::LoadFg& loadfg) : _loadfg(loadfg) {}

    bool handle_instruction(GuiInterface& gui) override
    {
        /// TODO: don't load them every time, use cache
        if (gui.fg_figures.empty()) {
            gui.fg_figures.emplace_back(
                    glm::ivec2{gui.parent_window->get_width() * 0.4, 0},
                    gui.parent_window->get_width() * 0.2,
                    gui.parent_window->get_height() * 0.8, *gui.parent_window,
                    vino::ImgData(
                            "res/" + std::string(_loadfg.path_fg.data())));
        } else {
            gui.fg_figures.front().change_texture(vino::ImgData(
                    "res/" + std::string(_loadfg.path_fg.data())));
        }
        gui.low_boxes.back().update_text(
                U"LOADFG: "
                + vino::convert_str<char, char32_t>(_loadfg.path_fg.data()));
        return false;
    }

private:
    vm_instr::LoadFg _loadfg;
};

class LoadTxtLineHandler : public IHandler {
public:
    explicit LoadTxtLineHandler(const vm_instr::LoadTxtLine& loadtxtline) :
        _txtline(loadtxtline)
    {
    }

    bool handle_instruction(GuiInterface& gui) override
    {
        // gui.low_boxes.back().update_text(U"LOADTXTLINE");
        gui.low_boxes.back().add_text(
                vino::to_utf32str(_txtline.txt_line.data()));
        // gui.low_boxes.back().update_text(U"Я челикс");
        return true;
    }

private:
    vm_instr::LoadTxtLine _txtline;
};

class PreBreakageHandler : public IHandler {
public:
    bool handle_instruction(GuiInterface&) override
    {
        return false;
    }
};

class TxtLineBreakageHandler : public IHandler {
public:

    bool handle_instruction(GuiInterface& gui) override
    {
        gui.low_boxes.back().next_slide();
        return true;
    }
};

class JmpHandler : public IHandler {
public:
    bool handle_instruction(GuiInterface& gui) override
    {
        // what can be here?
        gui.low_boxes.back().update_text(U"JMP");
        return true;
    }

private:
    // vm_instr::Jmp _jmp;
};

}  // namespace vino_vm