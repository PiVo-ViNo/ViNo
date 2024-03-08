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

#include <functional>
#include <memory>
#include <vector>

#include "AST.h"
#include "TokenEnum.h"

namespace vino {

/**
 @brief Analyzes the syntax of input `vector<ScriptToken>`
 @details Parser checks whether the vector of `ScriptTokens`, into which
 the input file/stream is divided by `TokenScanner`, is in compliance with
 syntax rules of ViNo Scripting Language.\n
 Soon: iterators instead of only vector.
 Usage: `parser.run()`
 @param get_token `std::function<PairTokenId>` which provide interface to get
 single token, it must not throw error if the end of input is reached, but
 simply give EXIT
*/
class Parser {
public:
    using func_type = std::function<PairTokenId()>;
    using token_ptr = std::unique_ptr<PairTokenId>;

    explicit Parser(func_type get_token_function) :
        get_tok_f(get_token_function)
    {
    }

    Parser(Parser &) = delete;

    Parser(Parser &&_p) :
        _cur_tok(std::move(_p._cur_tok)),
        _next_tok(std::move(_p._next_tok)),
        get_tok_f(std::move(_p.get_tok_f))
    {
    }

    Parser &operator=(const Parser &) = delete;

    //---------------Interface-------------------------------

    /**
    @brief Start parsing the tokens, checks if syntax is correct.
    @return AbstractSyntaxTree (root)
    @throw parsing_error() if syntax is incorrect
    @param verbose true: recommended only while debugging
    @warning in case PERSONA's braces have a new line symbol inside,
    Parser will delete them as it is just one line :
        @code persona {\n} --> persona {} @endcode
    */
    ScriptAst run(bool verbose = false);

    /**
    @brief Set new input, nullifies current line.
    @throw exception() if `vec_tokens` cannot be copied
    */
    void set_input(func_type get_token_function);

private:
    token_ptr   _cur_tok{};
    token_ptr   _next_tok{};
    func_type   get_tok_f;
    std::size_t _cur_line = 0;
    bool        _verb = false;

    //----------Private Methods----------------------------------
    //------------------------------------------------------------

    void actual_run(bool sym_table_set, bool verbose);

    /**
    @defgroup grammar Context-free Grammar constructions
    @{
    @details Parser is made as Context-free Grammar, basically
    it checks whether the tokens array follows the set pattern.
    Checks the file grammar_ideas.txt for a more formal definition.
    */
    inline ScriptAst script();

    StmtAst stmt();

    InsideAst inside();

    inline InsTypeAst type();
    /** @} */

    //-----------Utility Methods--------------------------------

    /**
    @brief Checks whether the token on position `_pos` is equal to `_tok` and
    increase `_pos`
    @return `{ScriptToken, ID}` of the matched token
    @throw parsing_error() in case it's not equal
    */
    inline void match(const ScriptToken &);

    inline void match_cur(const ScriptToken &);

    /**
    @brief Get the token on current position (`_pos`)
    @return ScriptToken from vector `_tokens_l`
    */
    inline void set_current_tok();
    inline void set_next_tok();

    /**
    @brief Get the token on current position (`_pos`) and
    move `_pos` on next token (`_pos+1`)
    @return ScriptToken from vector `_tokens_l`
    */
    inline PairTokenId &popout();
};

}  // namespace vino