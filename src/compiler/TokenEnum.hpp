/*
 * TokenEnum.hpp
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
    NAME,       // name in game dialogue
    TEXT_LINE,  // generic string of characters, could be path
    PATH,       // path to directory of PERSONA files
    //
    NEW_LINE,
    SIGN_EQ,
    BRACE_OP,
    BRACE_CL,
    COMMA,
    DOT,
    EXIT
};

std::string to_string(const ScriptToken &token);

std::ostream &operator<<(std::ostream &os, const ScriptToken &token);

struct PairTokenId {
    const ScriptToken token;
    const std::string id;

    PairTokenId(ScriptToken token, std::string id) :
        token(std::move(token)), id(std::move(id))
    {
    }
};

}  // namespace vino
