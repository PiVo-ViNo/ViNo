#include <ostream>

#include "TokenEnum.h"
#include "custom_errors.h"

std::ostream &vino::operator<<(std::ostream &os, const vino::ScriptToken &tok)
{
	switch (tok)
	{
	case vino::ScriptToken::EMPTY_TOKEN:
		os << "EMPTY_TOKEN";
		break;
	case vino::ScriptToken::BG:
		os << "BG";
		break;
	case vino::ScriptToken::FG:
		os << "FG";
		break;
	case vino::ScriptToken::TEXT_TYPE:
		os << "TEXT_TYPE";
		break;
	case vino::ScriptToken::PUT:
		os << "PUT";
		break;
	case vino::ScriptToken::PERSONA:
		os << "PERSONA";
		break;
	case vino::ScriptToken::VAR:
		os << "VAR";
		break;
	case vino::ScriptToken::NEW_LINE:
		os << "NEW_LINE";
		break;
	case vino::ScriptToken::SIGN_EQ:
		os << "SIGN_EQ";
		break;
	case vino::ScriptToken::BRACE_OP:
		os << "BRACE_OP";
		break;
	case vino::ScriptToken::BRACE_CL:
		os << "BRACE_CL";
		break;
	case vino::ScriptToken::COMMA:
		os << "COMMA";
		break;
	case vino::ScriptToken::TEXT_LINE:
		os << "TEXT_LINE";
		break;
	case vino::ScriptToken::NAME:
		os << "NAME";
		break;
	case vino::ScriptToken::PATH:
		os << "PATH";
		break;
	case vino::ScriptToken::EXIT:
		os << "EXIT";
		break;
	default:
		throw vino::tokenize_error("Unknown error while tokenizing\n");
		break;
	}
	return os;
}