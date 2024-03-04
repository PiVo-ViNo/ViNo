/*
 * TokenEnum.h
 *
 * Created on Fri Mar 01 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#pragma once

#include <ostream>

namespace vino
{

enum class ScriptToken
{
	EMPTY_TOKEN,
	// TYPE keyword
	BG,
	FG,
	TEXT_TYPE,
	PUT,
	PERSONA,
	//
	VAR,
	NEW_LINE,
	SIGN_EQ,
	BRACE_OP,
	BRACE_CL,
	COMMA,
	NAME,
	TEXT_LINE, // or WORD + QUOT_MARK ? what with \n and such?
	PATH,	   // maybe work with only TEXT_LINE? -- now it is used for directory of persona
	EXIT
};

std::ostream &operator<<(std::ostream &os, const ScriptToken &tok);
} // namespace vino