/*
 * TokenScanner.cpp
 *
 * required :
 *
 * Created on Thu Feb 29 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#include <cctype>
#include <iostream>
#include <memory>
#include <utility>
#if __cpp_lib_format
#include <format>
#endif

#include "TokenScanner.h"
#include "custom_errors.h"
#include "stuff.h"

namespace vino {

ScriptToken TokenScanner::is_keyword(const std::string &_str)
{
    if (insen_str_equal(_str, "background")) return ScriptToken::BG;
    if (insen_str_equal(_str, "foreground")) return ScriptToken::FG;
    if (insen_str_equal(_str, "text")) return ScriptToken::TEXT_TYPE;
    if (insen_str_equal(_str, "put")) return ScriptToken::PUT;
    if (insen_str_equal(_str, "persona")) return ScriptToken::PERSONA;
    if (insen_str_equal(_str, "path")) return ScriptToken::PATH;
    if (insen_str_equal(_str, "exit")) return ScriptToken::EXIT;
    if (insen_str_equal(_str, "name")) return ScriptToken::NAME;

    return ScriptToken::EMPTY_TOKEN;
}

Pair_TokenID TokenScanner::check_var_or_keyword(std::string &_str, char ch)
{
    _istream_ptr->putback(ch);

    ScriptToken new_token = is_keyword(_str);
    if (new_token != ScriptToken::EMPTY_TOKEN) {
        return {new_token, ""};
    }
    // it must be done a few levels higher in callstack
    // add_to_sym_table(_str);
    return {ScriptToken::VAR, _str};
}

Pair_TokenID TokenScanner::get_token()
{
    if (!_istream_ptr) throw NullPtrExc();

    /// Alphabet-numeric string, it gets all alnums from input
    std::string alnum_str{};

    char ch = 0;
    while ((ch = _istream_ptr->get()) && ch != -1) {
        switch (ch) {
            // skip comments entirely
            case '#':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '#');
                }
                do {
                    ch = _istream_ptr->get();
                    if (ch == '\n') {
                        _cur_line++;
                        return {ScriptToken::NEW_LINE, ""};
                    }
                } while (_istream_ptr->good() && ch != '#');
                break;

            case ' ':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, ' ');
                }
                break;

            case '{':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '{');
                }
                return {ScriptToken::BRACE_OP, ""};

            case '}':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '}');
                }
                return {ScriptToken::BRACE_CL, ""};

            case '=':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '=');
                }
                return {ScriptToken::SIGN_EQ, ""};

            case '\n':
                _cur_line++;
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '\n');
                }
                return {ScriptToken::NEW_LINE, ""};

            case ',':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, ',');
                }
                return {ScriptToken::COMMA, ""};

            case '.':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '.');
                }
                return {ScriptToken::DOT, ""};

            // {} for scope only initializing of prev_curline
            case '"': {
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '"');
                }
                std::size_t prev_curline = _cur_line;
                ch = _istream_ptr->get();
                while (_istream_ptr->good() && ch != '"') {
                    if (ch == '\n') _cur_line++;
                    // for now we just store text in Id
                    alnum_str += ch;
                    ch = _istream_ptr->get();
                }

                if (_istream_ptr->bad() || _istream_ptr->eof()) {
#if __cpp_lib_format
                    throw TokenizeError(
                        std::format("No enclosing \""
                                    "(quotation mark) in text on line {}\n",
                                    prev_curline));
#else
                    throw TokenizeError(
                        "No enclosing \" (quotation mark)"
                        "in text on line " +
                        std::to_string(prev_curline) + "\n");
#endif
                }
                // move(alnum_str) as it will be deleted anyways
                return {ScriptToken::TEXT_LINE,
                                      std::move(alnum_str)};
            }

            default:
                if (isalnum(ch)) {
                    alnum_str += ch;
                    break;
                } else {
#if __cpp_lib_format
                    throw TokenizeError(
                        std::format("Unrecognized symbol"
                                    "{} at line {}\n",
                                    ch, _cur_line));
#else
                    throw TokenizeError("Unrecognized symbol" +
                                        std::to_string(ch) + "at line " +
                                        std::to_string(_cur_line) + "\n");
#endif
                }
                break;
        }
    }

    if (!alnum_str.empty()) {
        return check_var_or_keyword(alnum_str, 0);
    }
    return {ScriptToken::EXIT, ""};
}

std::vector<Pair_TokenID> TokenScanner::get_all_tokens(bool verbose)
{
    std::vector<Pair_TokenID> tokens_vec;

    while (this->has_more_tokens()) {
        Pair_TokenID new_token_pair = get_token();
        if (verbose) std::cout << new_token_pair.first << '\n';
        tokens_vec.push_back(new_token_pair);
        if (new_token_pair.first == ScriptToken::EXIT) return tokens_vec;
    }
    return tokens_vec;
}

inline std::size_t TokenScanner::position()
{
    if (!_istream_ptr) throw NullPtrExc();
    return _istream_ptr->tellg();
}

inline bool TokenScanner::has_more_tokens()
{
    if (!_istream_ptr) throw NullPtrExc();
    return !(_istream_ptr->bad() || _istream_ptr->eof());
}

inline void TokenScanner::set_input(std::string &&input_str)
{
    _istream_ptr =
        std::make_unique<std::istringstream>(input_str, std::ios::in);
    _cur_line = 0;
}

inline void TokenScanner::set_input(std::ifstream &&input_file)
{
    _istream_ptr = std::make_unique<std::ifstream>(std::move(input_file));
    _cur_line = 0;
}

void TokenScanner::add_to_sym_table(const std::string &var) {}

}  // namespace vino
