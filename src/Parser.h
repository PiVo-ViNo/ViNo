/*
 * Parser.h
 *
 * required : 
 *
 * Created on Fri Mar 01 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#pragma once

#include <vector>

#include "TokenEnum.h"

namespace vino {

class Parser {

    Parser(const std::vector<ScriptToken>& vec_tokens) :
        _tokens_l(vec_tokens)
    {}

    Parser(Parser&) = delete;

    Parser(Parser&& _p) : _tokens_l(std::move(_p._tokens_l)),
        _pos(_p._pos), _line(_p._line)
    {}

private:

    std::vector<ScriptToken>    _tokens_l;
    std::size_t                 _pos = 0;
    std::size_t                 _line = 0;

    //----------Private Methods--------------------

    void script();

    void stmts();

    void stmt();

    inline void inside();

    inline void type();

    bool match(ScriptToken, bool throw_err = true);

    ScriptToken& popout();

};

}