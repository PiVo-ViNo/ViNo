/*
 * Parser.hpp
 *
 * required :
 *
 * Created on Fri Mar 01 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#pragma once

#include <functional>
#include <memory>
#include <utility>

#include "AST.hpp"
#include "TokenEnum.hpp"

namespace vino {

/**
 * @brief Analyzes the syntax of input tokens
 *
 * @details Parser checks whether the input of `ScriptTokens`, into which
 * the input file/stream is divided by `TokenScanner`, is in compliance with
 * syntax rules of ViNo Scripting Language.\n
 * Usage: `parser.run()`
 * @param get_token `std::function<PairTokenId()>` which provide interface to
 * get single token, it must not throw error if the end of input is reached, but
 * simply give EXIT
 */
class Parser {
public:
    using func_type = std::function<PairTokenId()>;
    using token_ptr = std::unique_ptr<PairTokenId>;

    explicit Parser(func_type get_token_function);

    Parser(Parser &&_p);

    Parser(Parser &) = delete;
    Parser &operator=(const Parser &) = delete;

    // Interface --------------------------------------------------------------
    //-------------------------------------------------------------------------
    /**
     * @brief Start parsing the tokens, checks if syntax is correct.
     *
     * @return ScriptAst The root of Abstract Syntax Tree
     * @throw `parsing_error()` if syntax is incorrect
     * @param verbose true: recommended only while debugging
     * @warning In case PERSONA's braces have a new line symbol inside,
     * Parser will delete them as it is just one line :
     *     @code persona {\n} --> persona {} @endcode
     */
    ScriptAst run(bool verbose = false);

    /// Set new input, nullifies current line.
    void set_input(func_type get_token_function);

private:
    token_ptr   _cur_tok{};
    func_type   get_tok_f;
    std::size_t _cur_line = 1;
    bool        _verb = false;

    // Grammar Structure Methods ----------------------------------------------
    //-------------------------------------------------------------------------
    /**
     * @defgroup grammar Context-free Grammar constructions
     * @{
     * @details Parser is made as Context-free Grammar, basically
     * it checks whether the input of tokens follows the set pattern.
     * Check GRAMMAR.md in the root directory for a more formal definition.
     */
    ScriptAst script();

    StmtAst stmt();

    InsideAst inside();

    InsTypeAst type();
    /** @} */

    // Utility Methods --------------------------------------------------------
    //-------------------------------------------------------------------------
    /**
     * @brief Checks whether the next token is equal to `tok`
     *
     * @throw ParsingError() in case it's not equal
     */
    void match(const ScriptToken &);

    /**
     * @brief Checks whether the current token is equal to `tok`
     *
     * @throw ParsingError() in case it's not equal
     */
    void match_cur(const ScriptToken &);

    /**
     * @brief Get the next token with `get_tok_f()`
     */
    void set_current_tok();
};

}  // namespace vino
