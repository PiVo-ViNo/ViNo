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

#include "TokenScanner.h"
#include "custom_errors.h"
#include "stuff.h"

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
    if (!_istream_ptr) 
        throw tokenize_error("No input stream\n");

    ScriptToken new_token{};
    std::string alnum_str{};

    while (char ch = _istream_ptr->get()) 
    {
        switch (ch) 
        {
        case '#':
            do {
                ch = _istream_ptr->get();
                if (ch == '\n') 
                {
                    cur_line++;
                    break;
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
        
        // {} for scope only initializing of prev_curline
        case '"': { 
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
        case '\n':
            if (!alnum_str.empty())
                return check_var_or_keyword(alnum_str, '\n');
            break;

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

inline long long TokenScanner::position() noexcept
{
    return _istream_ptr->tellg();
}

inline bool TokenScanner::has_more_tokens() noexcept
{
    return !(_istream_ptr->bad() || _istream_ptr->eof());
}

inline bool TokenScanner::set_input(std::string&& input_str)
{   
    _istream_ptr = 
        std::make_unique<std::istringstream>(input_str, std::ios::in);
}

inline bool TokenScanner::set_input(std::ifstream&& input_file)
{
    _istream_ptr = 
        std::make_unique<std::ifstream>(std::move(input_file));
} 

void TokenScanner::add_to_hash_map(std::string&& var)
{}