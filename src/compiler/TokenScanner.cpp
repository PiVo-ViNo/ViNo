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

#include "TokenScanner.hpp"

#include "custom_errors.hpp"
#include "stuff.hpp"
#include <cctype>
#include <iostream>
#include <memory>
#include <utility>

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

PairTokenId TokenScanner::check_var_or_keyword(std::string &_str, char ch)
{
    _istream_ptr->putback(ch);

    ScriptToken new_token = is_keyword(_str);
    if (new_token != ScriptToken::EMPTY_TOKEN) {
        return {new_token, ""};
    }
    return {ScriptToken::VAR, _str};
}

PairTokenId TokenScanner::get_token()
{
    if (!_istream_ptr) throw NullPtrExc();

    // Alphabet-numeric string, it gets all alnums from input
    std::string alnum_str{};

    char ch = 0;
    while (_istream_ptr->get(ch) //(ch = static_cast<char>(_istream_ptr->get()))
            && _istream_ptr->good())
    {
        switch (ch) {
            // skip comments entirely
            case '#':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '#');
                }
                do {
                    ch = static_cast<char>(_istream_ptr->get());
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

            case '\r':
                if (!alnum_str.empty()) {
                    return check_var_or_keyword(alnum_str, '\r');
                }
                break;

            case '\t':
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
                ch = static_cast<char>(_istream_ptr->get());
                while (_istream_ptr->good() && ch != '"') {
                    if (ch == '\n') {
                        _cur_line++;
                    }
                    // for now we just store text in Id
                    alnum_str += ch;
                    ch = static_cast<char>(_istream_ptr->get());
                }

                if (_istream_ptr->bad() || _istream_ptr->eof()) {
                    throw TokenizeError(
                            "No enclosing \" (quotation mark)"
                            "in text on line "
                            + std::to_string(prev_curline) + "\n");
                }
                // move(alnum_str) as it will be deleted anyways
                return {ScriptToken::TEXT_LINE, std::move(alnum_str)};
            }

            default:
                if (isalnum(ch)) {
                    alnum_str += ch;
                    break;
                } else {
                    throw TokenizeError("Unrecognized symbol "
                                        + std::to_string(ch) + " at line "
                                        + std::to_string(_cur_line) + "\n");
                }
                break;
        }
    }

    if (!alnum_str.empty()) {
        return check_var_or_keyword(alnum_str, 0);
    }
    return {ScriptToken::EXIT, ""};
}

std::vector<PairTokenId> TokenScanner::get_all_tokens(bool verbose)
{
    _verb = verbose;
    std::vector<PairTokenId> tokens_vec;

    while (this->has_more_tokens()) {
        PairTokenId new_token_pair = get_token();
        if (_verb) std::cout << new_token_pair.token << '\n';
        tokens_vec.push_back(new_token_pair);
        if (new_token_pair.token == ScriptToken::EXIT) return tokens_vec;
    }
    _verb = false;
    return tokens_vec;
}

std::vector<ScriptToken> TokenScanner::get_raw_tokens(bool verbose)
{
    _verb = verbose;
    std::vector<ScriptToken> tokens_vec;

    while (this->has_more_tokens()) {
        ScriptToken new_raw_token = get_token().token;
        if (_verb) std::cout << new_raw_token << '\n';
        tokens_vec.push_back(new_raw_token);
        if (new_raw_token == ScriptToken::EXIT) return tokens_vec;
    }
    _verb = false;
    return tokens_vec;
}

inline long long TokenScanner::position()
{
    if (!_istream_ptr) {
        throw NullPtrExc();
    }
    return _istream_ptr->tellg();
}

inline bool TokenScanner::has_more_tokens()
{
    if (!_istream_ptr) {
        throw NullPtrExc();
    }
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

TokenScanner::TokenScanner(std::ifstream &&in_filestream)
{
    _istream_ptr = std::make_unique<std::ifstream>(std::move(in_filestream));
}

TokenScanner::TokenScanner(std::string &&input_string)
{
    _istream_ptr =
            std::make_unique<std::istringstream>(input_string, std::ios::in);
}

TokenScanner::TokenScanner(TokenScanner &&other) :
    scan_whole_strs(other.scan_whole_strs)
{
    _istream_ptr = std::move(other._istream_ptr);
}

}  // namespace vino
