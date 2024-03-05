/*
 * Parser.cpp
 *
 * required :
 *
 * Created on Sat Mar 02 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

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

inline st &Parser::popout() { return _tokens_l.at(_pos++); }

inline void Parser::match(const st &_tok)
{
    st _cur_tok = popout();

    if (_verb) {
        std::cout << "match"
                  << "\t";
        std::cout << _tok << "\t";
        std::cout << _cur_tok << "\n";
    }
    if (_cur_tok != _tok)
#if __cpp_lib_format
        throw parsing_error(std::format("Parsing error on line {}\n",
                                        _cur_line));
#else
        throw parsing_error("Parsing error on line " + std::to_string(_cur_line)
                            + "\n");
#endif
}

void Parser::script()
{
    /*if (_tokens_l.at(_pos) == st::NEW_LINE) {
        _pos++;
        _cur_line++;
        if (_verb) {
            std::cout << "script" << _pos << " ";
        }
    }*/
    while (_tokens_l.at(_pos) != st::EXIT) {
        stmt();
    }
}

void Parser::stmt()
{
    st _tok = popout();

    if (_verb) {
        std::cout << "stmt\t" << _pos << _tok << "\t";
    }

    switch (_tok) {
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
            match(st::TEXT_LINE);
            break;

        case st::TEXT_TYPE:
            if (_verb) {
                std::cout << _tokens_l.at(_pos) << " ";
            }
            if (_tokens_l.at(_pos) == st::SIGN_EQ) {
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
        do {
            type();
            match(st::SIGN_EQ);
            match(st::TEXT_LINE);

        } while (/*_tokens_l.at(_pos)*/ popout() == st::COMMA);
        _pos--;
    } catch (std::out_of_range &) {
#if __cpp_lib_format
        throw ParsingError(std::format("Inside error at line {}\n", _cur_line));
#else
        throw ParsingError("Inside error at line " + std::to_string(_cur_line) +
                           "\n");
#endif
    }
}

inline void Parser::type()
{
    st _tok = popout();
    if (_verb) {
        std::cout << _tok << "\t" << _pos << "\n";
    }
    if (_tok == st::BG || _tok == st::FG || _tok == st::PATH ||
        _tok == st::NAME) {
        return;
    }
#if __cpp_lib_format
    throw ParsingError(std::format("Type error at line {}\n", _cur_line));
#else
    throw ParsingError("Type error at line " + std::to_string(_cur_line) +
                       "\n");
#endif

}

void Parser::set_input(const std::vector<st> &vec_tokens)
{
    _tokens_l = vec_tokens;
    _cur_line = 0;
    _pos = 0;
}

void Parser::run(bool verbose)
{
    _verb = verbose;
    if (_tokens_l.empty()) {
        throw ParsingError("Vector of tokens is not defined for this parser");
    }
    script();
}

}  // namespace vino
