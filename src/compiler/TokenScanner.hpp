/*
 * TokenScanner.hpp
 *
 * required :
 *
 * Created on Thu Feb 29 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */
#pragma once

#include "TokenEnum.hpp"

#include <fstream>
#include <istream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

namespace vino {

/**
 * @brief Divides the input stream/file into ScriptTokens.
 *
 * @details Usage: token_scanner.get_token(), token_scanner.get_all_tokens()
 * @param _istream_ptr unique_ptr<istream>, can be string/istringstream
 * or ifstream, constructor() or set_input() allow only rvalue reference&&.
 */
class TokenScanner {
public:
    TokenScanner() = default;

    explicit TokenScanner(std::ifstream &&in_filestream);

    explicit TokenScanner(std::string &&input_string);

    TokenScanner(TokenScanner &) = delete;

    TokenScanner(TokenScanner &&other);

    // Interface --------------------------------------------------------------
    //-------------------------------------------------------------------------

    inline void set_input(std::string &&);

    inline void set_input(std::ifstream &&);

    /**
     * @throw tokenize_error
     * @return ScriptToken (i.e. enum class)
     */
    PairTokenId get_token();

    /**
     * @brief Get all tokens in one step in case you don't want to bother
     *
     * @return `std::vector<vino::PairTokenId>`
     * @throw tokenize_error() or any exception within std::vector
     */
    std::vector<PairTokenId> get_all_tokens(bool verbose = false);

    /**
     * @brief Get all tokens without `id`
     * @return `std::vector<vino::ScriptToken>`
     */
    std::vector<ScriptToken> get_raw_tokens(bool verbose = false);

    /**
     * @brief Get current position in the input stream
     *
     * @return -1 if false() is true, otherwise any integer
     * @throw NullPtrExc() if the input stream isn't set
     */
    inline long long position();

    /**
     * @throw NullPtrExc() if the input stream isn't set
     */
    inline bool has_more_tokens();

    // Flags switchers --------------------------------------------------------
    //-------------------------------------------------------------------------

    // not used for now
    // void scan_strings_whole();

private:
    // std::unique_ptr<std::istream> _prev_istream_ptr;
    std::unique_ptr<std::istream> _istream_ptr;
    std::size_t                   _cur_line = 0;

    bool scan_whole_strs = true;
    bool _verb = false;
    // bool            _throw_exceptions = true; ?

    ScriptToken is_keyword(const std::string &);

    /**
     * @brief Strings found in input is either keyword or variable name
     * @param `ch` is the char that must be put back into istream
     */
    PairTokenId check_var_or_keyword(std::string &, char);
};

}  // namespace vino
