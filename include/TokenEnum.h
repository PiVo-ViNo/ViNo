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

#include "custom_errors.h"

namespace vino {

enum class ScriptToken {
    EMPTY_TOKEN,
    // TYPE keyword
    BG, FG, TEXT_TYPE, PUT, PERSONA,
    //
    VAR,
    NEW_LINE,
    SIGN_EQ,
    BRACE_OP,
    BRACE_CL,
    COMMENT, // comment from # to either # or \n (NEW_L) 
    TEXT_LINE, // or WORD + QUOT_MARK ? what with \n and such?
    PATH, // maybe work with only TEXT_LINE?
    EXIT
};

std::ostream& operator<<(std::ostream& os, const ScriptToken& tok)
{
    switch (tok)
    {
    case ScriptToken::EMPTY_TOKEN: os << "EMPTY_TOKEN"; break;
    case ScriptToken::BG: os << "BG"; break;
    case ScriptToken::FG: os << "FG"; break;
    case ScriptToken::TEXT_TYPE: os << "TEXT_TYPE"; break;
    case ScriptToken::PUT: os << "PUT"; break;
    case ScriptToken::PERSONA: os << "PERSONA"; break;
    case ScriptToken::VAR: os << "VAR"; break;
    case ScriptToken::NEW_LINE: os << "NEW_LINE"; break;
    case ScriptToken::SIGN_EQ: os << "SIGN_EQ"; break;
    case ScriptToken::BRACE_OP: os << "BRACE_OP"; break;
    case ScriptToken::BRACE_CL: os << "BRACE_CL"; break;
    case ScriptToken::COMMENT: os << "COMMENT"; break;
    case ScriptToken::TEXT_LINE: os << "TEXT_LINE"; break;
    case ScriptToken::PATH: os << "PATH"; break;
    case ScriptToken::EXIT: os << "EXIT"; break;
    default: throw tokenize_error("Unknown error while tokenizing\n"); break;
    }
    return os;
}

}