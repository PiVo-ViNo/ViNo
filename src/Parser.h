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

/// @brief Analyzes the syntax of input vector<ScriptToken>
/// @details Parser checks whether the vector of ScriptTokens, into which 
/// the input file/stream is divided by TokenScanner, is in compliance with
/// syntax rules of ViNo Scripting Language.\n 
///
/// Soon: iterators instead of only vector.
/// Usage: parser.run()
class Parser {
public:

	Parser() {}

    explicit Parser(const std::vector<ScriptToken> &vec_tokens) :
        _tokens_l(vec_tokens) {}

    Parser(Parser &) = delete;

    Parser(Parser &&_p) : _tokens_l(std::move(_p._tokens_l)) {}

    //---------------Interface-------------------------------

    /// @brief Start parsing the tokens, checks if syntax is correct.
    /// @throw parsing_error() if syntax is incorrect
    /// @param verbose true: recommended only while debugging
    void run(bool verbose = false);

    /// @brief Set new input, nullifies current line.
    /// @throw exception() if vec_tokens cannot be copied
    void set_input(const std::vector<ScriptToken> &vec_tokens);

private:
    std::vector<ScriptToken> _tokens_l{};
    std::size_t _pos  = 0;
    std::size_t _cur_line = 0;
    bool _verb        = false;

    //----------Private Methods----------------------------------
    //------------------------------------------------------------

    //------------Grammar-----------------------------------------
    /// Description: Parser is made as Context-free Grammar, basically 
    /// it checks whether the tokens array follows the set pattern.
    /// Checks the file grammar_ideas.txt for a more formal definition.

    //------------Grammar-----------------------------------------
    /// Description: Parser is made as Context-free Grammar, basically 
    /// it checks whether the tokens array follows the set pattern.
    /// Checks the file grammar_ideas.txt for a more formal definition.

    /// 
    void script();

    void stmt();

    inline void inside();

    inline void type();

    //-----------Utility Methods--------------------------------

    /// @brief Checks whether the token on position _pos is equal to _tok
    /// @throw parsing_error() in case it's not equal 
    inline void match(const ScriptToken &);

    /// @brief Get the token on position _pos
    /// @return ScriptToken from vector _tokens_l
    inline ScriptToken &popout();
};

}  // namespace vino