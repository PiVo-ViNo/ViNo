#include "TokenEnum.hpp"
#include "custom_errors.hpp"

std::ostream& vino::operator<<(std::ostream& os, const vino::ScriptToken& tok) {
    using vst = vino::ScriptToken;

    switch (tok) {
        case vst::EMPTY_TOKEN:
            os << "EMPTY_TOKEN";
            break;
        case vst::VAR:
            os << "VAR";
            break;
        case vst::BG:
            os << "BG";
            break;
        case vst::FG:
            os << "FG";
            break;
        case vst::TEXT_TYPE:
            os << "TEXT_TYPE";
            break;
        case vst::PUT:
            os << "PUT";
            break;
        case vst::PERSONA:
            os << "PERSONA";
            break;
        case vst::TEXT_LINE:
            os << "TEXT_LINE";
            break;
        case vst::NAME:
            os << "NAME";
            break;
        case vst::PATH:
            os << "PATH";
            break;
        case vst::NEW_LINE:
            os << "NEW_LINE";
            break;
        case vst::SIGN_EQ:
            os << "SIGN_EQ";
            break;
        case vst::BRACE_OP:
            os << "BRACE_OP";
            break;
        case vst::BRACE_CL:
            os << "BRACE_CL";
            break;
        case vst::COMMA:
            os << "COMMA";
            break;
        case vst::DOT:
            os << "DOT";
            break;
        case vst::EXIT:
            os << "EXIT";
            break;
        default:
            throw vino::TokenizeError("Unknown error while tokenizing\n");
            break;
    }
    return os;
}
