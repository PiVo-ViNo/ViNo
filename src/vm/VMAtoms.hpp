/*
 * VMAtoms.hpp
 *
 * Created on Thu Apr 04 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */
#pragma once

#include <array>
#include <cstdint>

/// TODO:Q: Windows use wchar for paths, is it breaking all algorithm or not?
namespace vm_instr {

struct Nop {};

struct Start {
    uint64_t jmp_line{};
};

struct Exit {
    uint8_t err_code{};
};

struct LoadBg {
    std::array<uint8_t, 64> path_bg{};
};

struct ClearBg {};

struct LoadFg {
    std::array<uint8_t, 64> path_fg{};
};
/*
struct LoadFgPos {
}; */
/*
struct AddFg {

}; */

struct LoadTxtLine {
    std::array<uint8_t, 64> txt_line{};
};

struct Jmp {
    uint64_t where{};
};

}  // namespace vm_instr