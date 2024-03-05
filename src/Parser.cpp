///////////////////////////////////////////////////////////////////////////////
// Parser.cpp                   //    /^--^\     /^--^\     /^--^\           //
//                              //    \____/     \____/     \____/           //
// Created on Sat Mar 02 2024   //   /      \   /      \   /      \          //
//                              //  |        | |        | |        |         //
//                              //   \__  __/   \__  __/   \__  __/          //
//                              //|^|^|^\ \^|^|^|^/ /^|^|^|^|^\ \^|^|^|^|^|^|//
//                              //| | | |\ \| | |/ /| | | | | | \ \ | | | | |//
// (c) Andrjusha (aka SibJusha) //| | | / / | | |\ \| | | | | |/ /| | | | | |//
//                              //| | | \/| | | | \/| | | | | |\/ | | | | | |//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <stdexcept>
#include <vector>

#if __cpp_lib_format
#include <format>
#endif

#include "Parser.h"
#include "TokenEnum.h"
#include "custom_errors.h"

namespace vino {

using st = ScriptToken;

inline Pair_TokenID &Parser::cur_tok() { return _tokens_l.at(_pos); }

inline Pair_TokenID &Parser::popout() { return _tokens_l.at(_pos++); }

inline void Parser::match(const ScriptToken &_tok)
{
    Pair_TokenID _cur_tok = popout();

    if (_verb) {
        std::cout << "match"
                  << "\t";
        std::cout << _tok << "\t";
        std::cout << _cur_tok.first << "\n";
    }
    if (_cur_tok.first != _tok) {
#if __cpp_lib_format
        throw ParsingError(
            std::format("Parsing error on line {}\n", _cur_line));
#else
        throw ParsingError("Parsing error on line " +
                           std::to_string(_cur_line) + "\n");
#endif
    }
}

void Parser::script()
{
    if (_verb) {
        std::cout << "script" << _pos << " ";
    }
    while (cur_tok().first != st::EXIT) {
        stmt();
    }
}

void Parser::stmt()
{
    Pair_TokenID _tok = popout();

    if (_verb) {
        std::cout << "stmt\t" << _pos << "\t" << _tok.first << "\n";
    }

    switch (_tok.first) {
        case st::PERSONA:
            match(st::VAR);
            match(st::BRACE_OP);
            inside();
            match(st::BRACE_CL);
            break;

        case st::BG:
            match(st::SIGN_EQ);
            match(st::TEXT_LINE);
            break;

        case st::FG:
            match(st::SIGN_EQ);
            if (cur_tok().first == st::VAR) {
                _pos++;
                match(st::DOT);
                match(st::VAR);
            }
            match(st::TEXT_LINE);
            break;

        case st::TEXT_TYPE:
            if (_verb) {
                std::cout << cur_tok().first << " ";
            }
            if (cur_tok().first == st::SIGN_EQ) {
                _pos++;
                match(st::TEXT_LINE);
            } else {
                match(st::TEXT_LINE);
            }
            break;

        case st::NEW_LINE:
            _cur_line++;
            return;

        default:
            throw ParsingError("Stmt error\n");
            break;
    }

    match(st::NEW_LINE);
    _cur_line++;
}

void Parser::inside()
{
    try {
        type();
        match(st::SIGN_EQ);
        match(st::TEXT_LINE);

        while (popout().first == st::COMMA) {
            if (cur_tok().first == st::NEW_LINE) _pos++;
            type();
            match(st::SIGN_EQ);
            match(st::TEXT_LINE);
        }
        _pos--;

    } catch (std::out_of_range &) {
#if __cpp_lib_format
        throw ParsingError(
            std::format("Inside braces error at line {}\n", _cur_line));
#else
        throw ParsingError("Inside braces error at line " +
                           std::to_string(_cur_line) + "\n");
#endif
    }
}
// changes : now BG can't be tied to PERSONA, but different VARs as subs can
inline void Parser::type()
{
    Pair_TokenID _tok = popout();
    if (_verb) {
        std::cout << _tok.first << "\t" << _pos << "\n";
    }
    if (_tok.first == st::FG || _tok.first == st::PATH ||
        _tok.first == st::NAME || _tok.first == st::VAR) {
        return;
    }
#if __cpp_lib_format
    throw ParsingError(
        std::format("Error in 'persona' at line {}:\n-Only 'background', "
                    "'foreground', 'path' and variable allowed inside\n",
                    _cur_line));
#else
    throw ParsingError("Error in 'persona' at line " +
                       std::to_string(_cur_line) +
                       ":\n-Only 'background', 'foreground', 'path' and "
                       "variable are allowed inside\n");
#endif
}

void Parser::set_input(const std::vector<Pair_TokenID> &vec_tokens)
{
    _tokens_l = vec_tokens;
    _cur_line = 0;
    _pos = 0;
}

void Parser::run(bool verbose)
{
    _verb = verbose;
    if (_tokens_l.empty()) {
        throw ParsingError("Vector of tokens is not defined for this parser\n");
    }
    script();
}

}  // namespace vino
