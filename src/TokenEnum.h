/*
 * TokenEnum.h
 *
 * Created on Fri Mar 01 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#pragma once

#include <ostream>
#include <utility>

namespace vino {

enum class ScriptToken {
    EMPTY_TOKEN,
    VAR,
    // TYPE keyword
    BG,
    FG,
    TEXT_TYPE,
    PUT,
    PERSONA,
    NAME,   // name in game dialogue
    TEXT_LINE,// generic string of characters, could be path
    PATH,  // path to directory of PERSONA files
    //
    NEW_LINE,
    SIGN_EQ,
    BRACE_OP,
    BRACE_CL,
    COMMA,
    DOT,
    EXIT
};

std::ostream &operator<<(std::ostream &os, const ScriptToken &tok);

typedef std::pair<ScriptToken, std::string> Pair_TokenID;

}  // namespace vino