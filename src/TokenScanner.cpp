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

#include <memory>
#include <cctype>
#include <format>
#include <iostream>
#include <utility>

#include "TokenScanner.h"
#include "custom_errors.h"
#include "stuff.h"

namespace vino {

std::ostream& operator<<(std::ostream& os, const ScriptToken& tok)
{
    switch (tok)
    {
    case ScriptToken::EMPTY_TOKEN: os << "EMPTY_TOKEN"; break;
    case ScriptToken::BG: os << "BG"; break;
    case ScriptToken::FG: os << "FG"; break;
    case ScriptToken::TEXT_TYPE: os << "TEXT_TYPE"; break;
    case ScriptToken::PUT: os << "PUT"; break;
    case ScriptToken::PERSONA: os << "PERSONA"; break;
    case ScriptToken::VAR: os << "VAR"; break;
    case ScriptToken::NEW_LINE: os << "NEW_LINE"; break;
    case ScriptToken::SIGN_EQ: os << "SIGN_EQ"; break;
    case ScriptToken::BRACE_OP: os << "BRACE_OP"; break;
    case ScriptToken::BRACE_CL: os << "BRACE_CL"; break;
    case ScriptToken::COMMENT: os << "COMMENT"; break;
    case ScriptToken::TEXT_LINE: os << "TEXT_LINE"; break;
    case ScriptToken::PATH: os << "PATH"; break;
    case ScriptToken::EXIT: os << "EXIT"; break;
    default: throw tokenize_error("Unknown error while tokenizing\n"); break;
    }
    return os;
}

ScriptToken TokenScanner::is_keyword(const std::string& _str)
{
    if (insen_str_equal(_str, "background"))
        return ScriptToken::BG;
    if (insen_str_equal(_str, "foreground"))
        return ScriptToken::FG;
    if (insen_str_equal(_str, "text"))
        return ScriptToken::TEXT_TYPE;
    if (insen_str_equal(_str, "put"))
        return ScriptToken::PUT;
    if (insen_str_equal(_str, "persona"))
        return ScriptToken::PERSONA;
    if (insen_str_equal(_str, "exit"))
        return ScriptToken::EXIT;

    return ScriptToken::EMPTY_TOKEN;
}

ScriptToken TokenScanner::check_var_or_keyword(std::string& _str, char ch)
{
    ScriptToken new_token = is_keyword(_str);
    if (new_token != ScriptToken::EMPTY_TOKEN)
        return new_token;

    _istream_ptr->putback(ch);
    add_to_hash_map(std::move(_str));

    return ScriptToken::VAR;
}

ScriptToken TokenScanner::get_token()
{
    if (!_istream_ptr) throw null_ptr_exc();

    std::string alnum_str{};

    while (char ch = _istream_ptr->get()) 
    {
        switch (ch) 
        {
        // skip comments entirely
        case '#':
            if (!alnum_str.empty())
                return check_var_or_keyword(alnum_str, '#');   
            do {
                ch = _istream_ptr->get();
                if (ch == '\n') 
                {
                    cur_line++;
                    return ScriptToken::NEW_LINE;
                }
            } while (_istream_ptr->good() && ch != '#');
            break;

        case ' ':
            if (!alnum_str.empty())
                return check_var_or_keyword(alnum_str, ' ');
            break;

        case '{':
            if (!alnum_str.empty()) 
                return check_var_or_keyword(alnum_str, '{');
            return ScriptToken::BRACE_OP;

        case '}':
            if (!alnum_str.empty()) 
                return check_var_or_keyword(alnum_str, '}');
            return ScriptToken::BRACE_CL;

        case '=':
            if (!alnum_str.empty())
                return check_var_or_keyword(alnum_str, '=');
            return ScriptToken::SIGN_EQ;

        case '\n':
            cur_line++;
            if (!alnum_str.empty())
                return check_var_or_keyword(alnum_str, '\n');
            break;
        
        // {} for scope only initializing of prev_curline
        case '"': { 
            if (!alnum_str.empty())
                return check_var_or_keyword(alnum_str, '"');
            std::size_t prev_curline = cur_line;
            do {
                ch = _istream_ptr->get();
                if (ch == '\n')
                    cur_line++;
                /* ... 
                 * save raw text somewhere / what else can be done?
                 * ...
                 */
            } while (_istream_ptr->good() && ch != '"');

            if (_istream_ptr->bad() || _istream_ptr->eof())
                throw tokenize_error(std::format("No enclosing \"" 
                                    "(quotaion mark) in text on line {}\n",
                                    prev_curline)
                                    );

            return ScriptToken::TEXT_LINE; 
        }

        default:
            if(isalnum(ch))
            {
                alnum_str += ch;
                break;
            } 
            else 
                throw tokenize_error(std::format("Unrecognized symbol"
                                    "{} at line {}\n", ch, cur_line)
                                    );
            break;
        }
    }

    if (!alnum_str.empty())
        return check_var_or_keyword(alnum_str, 0);

    return ScriptToken::EXIT;
}

std::vector<ScriptToken> TokenScanner::get_all_tokens(bool verbose)
{
    std::vector<ScriptToken> tokens_vec;
    while (this->has_more_tokens())
    {
        ScriptToken new_token = get_token();
        if (verbose) 
            std::cout << new_token << '\n';
        tokens_vec.push_back(new_token);
    }
    return tokens_vec;
}

inline long long TokenScanner::position()
{
    if (!_istream_ptr) throw null_ptr_exc();
    return _istream_ptr->tellg();
}

inline bool TokenScanner::has_more_tokens()
{
    if (!_istream_ptr) throw null_ptr_exc();
    return !(_istream_ptr->bad() || _istream_ptr->eof());
}

inline void TokenScanner::set_input(std::string&& input_str)
{   
    _istream_ptr = 
        std::make_unique<std::istringstream>(input_str, std::ios::in);
}

inline void TokenScanner::set_input(std::ifstream&& input_file)
{
    _istream_ptr = 
        std::make_unique<std::ifstream>(std::move(input_file));
} 

void TokenScanner::add_to_hash_map(std::string&& var)
{}

}