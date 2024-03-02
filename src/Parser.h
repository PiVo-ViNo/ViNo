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

    Parser(Parser&& _p) : _tokens_l(std::move(_p._tokens_l))
    {}

    //---------------Interface-------------------------------

    /* @throw parsing_error() if syntax is incorrect
     */
    void run(bool verbose = false);

    void set_input(const std::vector<ScriptToken>& vec_tokens);

private:

    std::vector<ScriptToken>    _tokens_l;
    std::size_t                 _pos = 0;
    std::size_t                 _line = 0;
    bool                        _verb = false;

    //----------Private Methods--------------------

    void script();

    void stmt();

    inline void inside();

    inline void type();

    inline void match(const ScriptToken&);

    inline ScriptToken& popout();

};

}