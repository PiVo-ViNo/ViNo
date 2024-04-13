/*
 * VMAtoms.hpp
 *
 * Created on Thu Apr 04 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */
/**
 * @file Instructions for binary language used in ViNo Virtual Machine
 */
#pragma once

#include <array>
#include <cstdint>

/// TODO:Q: Windows use wchar for paths, is it breaking all algorithm or not?
namespace vm_instr {

/**
 * @defgroup instructions VM Instructions
 * @{
 * @details Full list of instructions with opcodes, instructions with no struct
 * members doesn't have any parameters beside opcode.
 */
/**
 * @brief NOP instruction 0x0
 */
struct Nop {};

/**
 * @brief START instruction 0x1
 * @param jmp_pos uint32_t Where start
 */
struct Start {
    uint32_t start_pos{};
};

/**
 * @brief EXIT instruction 0x2
 * @param err_code uint8_t Error code:
 *      0 - no error
 */
struct Exit {
    char err_code{};
};

/**
 * @brief LOADBG instruction 0x10
 * @param bath_bg uint8_t[64] Path/to/bg
 */
struct LoadBg {
    std::array<char, 64> path_bg{};
};

/**
 * @brief CLEARBG instruction 0x11
 */
struct ClearBg {};

/**
 * @brief LOADFG instruction 0x20
 * @param path_fg uint8_t[64] Path/to/fg
 */
struct LoadFg {
    std::array<char, 64> path_fg{};
};
/*
struct LoadFgPos {
}; */
/*
struct AddFg {

}; */

/**
 * @brief LOADTXTLINE instruction 0x30
 * @param txt_line char[64] String
 */
struct LoadTxtLine {
    std::array<char, 64> txt_line{};
};

/**
 * @brief PREBREAKAGE instruction 0x31
 *
 * @details Instruction to signal VM that end of TXTLINE is reached
 * and next instruction will be actual TxtLineBreakage (0x32)
 */
struct PreBreakage {};

/**
 * @brief TXTLINEBREAKAGE instruction 0x32
 *
 * @details Instruction to signal VM to break line in text box.
 * Useful for texts in multiple lines.
 */
struct TxtLineBreakage {};

/**
 * @brief JMP instruction 0x40
 * @param where_pos uint32_t Byte position where to jump
 */
struct Jmp {
    uint32_t where_pos{};
};

/* @}*/

}  // namespace vm_instr