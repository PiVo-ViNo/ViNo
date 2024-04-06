#include "TokenEnum.hpp"
#include "custom_errors.hpp"

namespace vino {

std::string to_string(const ScriptToken& token)
{
    switch (token) {
        case ScriptToken::EMPTY_TOKEN:
            return "EMPTY_TOKEN";
        case ScriptToken::VAR:
            return "VAR";
        case ScriptToken::BG:
            return "BG";
        case ScriptToken::FG:
            return "FG";
        case ScriptToken::TEXT_TYPE:
            return "TEXT_TYPE";
        case ScriptToken::PUT:
            return "PUT";
        case ScriptToken::PERSONA:
            return "PERSONA";
        case ScriptToken::TEXT_LINE:
            return "TEXT_LINE";
        case ScriptToken::NAME:
            return "NAME";
        case ScriptToken::PATH:
            return "PATH";
        case ScriptToken::NEW_LINE:
            return "NEW_LINE";
        case ScriptToken::SIGN_EQ:
            return "SIGN_EQ";
        case ScriptToken::BRACE_OP:
            return "BRACE_OP";
        case ScriptToken::BRACE_CL:
            return "BRACE_CL";
        case ScriptToken::COMMA:
            return "COMMA";
        case ScriptToken::DOT:
            return "DOT";
        case ScriptToken::EXIT:
            return "EXIT";
        default:
            throw TokenizeError("Unknown error while tokenizing\n");
    }
}

std::ostream& operator<<(std::ostream& os, const ScriptToken& token)
{
    os << to_string(token);
    return os;
}

}  // namespace vino