/*
 * Parser.cpp
 *
 * required :
 *
 * Created on Sat Mar 02 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#include <iostream>
#include <vector>
#if __cplusplus >= 202002L
#include <format>
#endif

#include "Parser.h"
#include "custom_errors.h"

namespace vino
{

using st = ScriptToken;

inline st &Parser::popout()
{
	_pos++;
	return _tokens_l.front();
}

inline void Parser::match(const st &_tok)
{
	st _cur_tok = popout();
	std::cout << _tok << " ";
	if (_cur_tok != _tok)
		throw vino::parsing_error();
}

void Parser::script()
{
	if (_tokens_l.front() == st::NEW_LINE)
		_pos++;
	do
	{
		stmt();
	} while (_tokens_l.at(_pos) != st::EXIT);
}

void Parser::stmt()
{
	st _tok = popout();

	switch (_tok)
	{
	case st::PERSONA:
		match(st::VAR);
		match(st::BRACE_OP);
		inside();
		match(st::BRACE_CL);
		break;

	case st::BG:
		match(st::TEXT_LINE);
		break;

	case st::FG:
		match(st::TEXT_LINE);
		break;

	case st::TEXT_TYPE:
		if (_verb)
			std::cout << _tokens_l.front() << " ";
		if (_tokens_l.front() == st::SIGN_EQ)
		{
			_pos++;
			match(st::TEXT_LINE);
		}
		else
			match(st::TEXT_LINE);
		break;

	default:
		throw vino::parsing_error("Stmt error\n");
		break;
	}

	match(st::NEW_LINE);
}

void Parser::inside()
{
	st _tok = popout();

	try
	{
		do
		{
			type();
			match(st::SIGN_EQ);
			match(st::TEXT_LINE);
		} while (_tokens_l.at(_pos) == st::COMMA);
	}
	catch (std::out_of_range &)
	{
#if __cplusplus >= 202002L
		throw vino::parsing_error(std::format("Inside error at line {}\n", _line));
#else
		throw parsing_error("Inside error at line " + std::to_string(_line) + "\n");
#endif
	}
}

inline void Parser::type()
{
	st _tok = popout();
	if (_tok == st::BG || _tok == st::FG || _tok == st::PATH || _tok == st::NAME)
		return;
#if __cplusplus >= 202002L
	throw vino::parsing_error(std::format("Type error at line {}\n", _line));
#else
	throw parsing_error("Type error at line " + std::to_string(_line) + "\n");
#endif
}

void Parser::set_input(const std::vector<st> &vec_tokens)
{
	_tokens_l = vec_tokens;
}

void Parser::run(bool verbose)
{
	_verb = verbose;
	script();
}

} // namespace vino
