/*
 * TokenScanner.h
 *
 * required :
 *
 * Created on Thu Feb 29 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */
#pragma once

#include <fstream>
#include <istream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "TokenEnum.h"

namespace vino {
    
/// @brief Divides the input stream/file into ScriptTokens.
/// Usage: token_scanner.get_token(), token_scanner.get_all_tokens()
/// @param _istream_ptr unique_ptr<istream>, can be string/istringstream
/// or ifstream, constructor() or set_input() allow only rvalue reference&&.
class TokenScanner {
public:
    TokenScanner() {}

    explicit TokenScanner(std::ifstream &&in_filestream) {
        _istream_ptr =
            std::make_unique<std::ifstream>(std::move(in_filestream));
    }

    explicit TokenScanner(std::string &&input_string) {
        _istream_ptr =
            std::make_unique<std::istringstream>(input_string, std::ios::in);
    }

    TokenScanner(TokenScanner &) = delete;

    TokenScanner(TokenScanner &&other) :
        scan_whole_strs(other.scan_whole_strs) {
        _istream_ptr = std::move(other._istream_ptr);
    }

    // ------------- Interface ------------------------------------------------

    inline void set_input(std::string &&);

    inline void set_input(std::ifstream &&);

    /// @throw tokenize_error
    /// @return ScriptToken (i.e. enum class)
    ScriptToken get_token();

    /// @brief Get all tokens in one step in case you don't want to bother
    /// @return rvalue reference to std::vector<ScriptToken>
    /// @throw tokenize_error() or any exception within std::vector
    std::vector<ScriptToken> get_all_tokens(bool verbose = false);

    /// @brief Get current position in the input stream
    /// @return -1 if false() is true, otherwise any integer
    /// @throw null_ptr_exc() if the input stream isn't set
    inline std::size_t position();

    /// @throw null_ptr_exc() if the input stream isn't set
    inline bool has_more_tokens();

    // -------------- Flags switchers ------------------------------------------

    // not used for now
    void scan_strings_whole();

private:
    std::unique_ptr<std::istream> _prev_istream_ptr;
    std::unique_ptr<std::istream> _istream_ptr;
    std::size_t cur_line = 0;
    bool scan_whole_strs = true;
    // bool            _throw_exceptions = true; ?

    // -------------- Private Methods -----------------------------------------

    ScriptToken is_keyword(const std::string &);

    /// @brief Strings found in input is either keyword or variable name
    /// @param ch is the char that must be put back into istream
    ScriptToken check_var_or_keyword(std::string &, char);

    // dummy function for now
    void add_to_sym_table(std::string &&var);
};

}  // namespace vino