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

ScriptToken TokenScanner::check_var_or_keyword(std::string &_str, char ch)
{
    _istream_ptr->putback(ch);
 
    ScriptToken new_token = is_keyword(_str);
    if (new_token != ScriptToken::EMPTY_TOKEN) return new_token;

    add_to_sym_table(std::move(_str));
    return ScriptToken::VAR;
}

ScriptToken TokenScanner::get_token()
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
                        cur_line++;
                        return ScriptToken::NEW_LINE;
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
                return ScriptToken::BRACE_OP;

            case '}':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '}');
                }
                return ScriptToken::BRACE_CL;

            case '=':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '=');
                }
                return ScriptToken::SIGN_EQ;

            case '\n':
                cur_line++;
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '\n');
                }
                return ScriptToken::NEW_LINE;

            case ',':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '\n');
                }
                return ScriptToken::COMMA;

            // {} for scope only initializing of prev_curline
            case '"': {
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '"');
                }
                std::size_t prev_curline = cur_line;
                do {
                    ch = _istream_ptr->get();
                    if (ch == '\n') cur_line++;
                    /* ...
                     * save raw text somewhere / what else can be done?
                     * ...
                     */
                } while (_istream_ptr->good() && ch != '"');

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

                return ScriptToken::TEXT_LINE;
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
                                    ch, cur_line));
#else
                    throw TokenizeError("Unrecognized symbol" +
                                        std::to_string(ch) + "at line " +
                                        std::to_string(cur_line) + "\n");
#endif
                }
                break;
        }
    }

    if (!alnum_str.empty()) {
        return check_var_or_keyword(alnum_str, 0);
    }

    return ScriptToken::EXIT;
}

std::vector<ScriptToken> TokenScanner::get_all_tokens(bool verbose)
{
    std::vector<ScriptToken> tokens_vec;

    while (this->has_more_tokens()) {
        ScriptToken new_token = get_token();
        if (verbose) std::cout << new_token << '\n';
        tokens_vec.push_back(new_token);
        if (new_token == ScriptToken::EXIT) return tokens_vec;
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
}

inline void TokenScanner::set_input(std::ifstream &&input_file)
{
    _istream_ptr = std::make_unique<std::ifstream>(std::move(input_file));
}

void TokenScanner::add_to_sym_table(std::string &&var) {}

}  // namespace vino
