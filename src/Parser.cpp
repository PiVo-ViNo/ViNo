///////////////////////////////////////////////////////////////////////////////
// Parser.cpp                   //    /^--^\     /^--^\     /^--^\           //
//                              //    \____/     \____/     \____/           //
// Created on Sat Mar 02 2024   //   /      \   /      \   /      \          //
//                              //  |        | |        | |        |         //
//                              //   \__  __/   \__  __/   \__  __/          //
//                              //|^|^|^\ \^|^|^|^/ /^|^|^|^|^\ \^|^|^|^|^|^|//
//                              //| | | |\ \| | |/ /| | | | | |\ \ | | | | | //
// (c) Andrjusha (aka SibJusha) //| | |  / / | | \ \| | | | | |/ /| | | | | |//
//                              //| | |  \/| | |  \/| | | | | |\/ | | | | | |//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <memory>

#if __cpp_lib_format
#include <format>
#endif

#include "Parser.h"
#include "TokenEnum.h"
#include "custom_errors.h"

namespace vino {

using st = ScriptToken;

inline void Parser::set_current_tok()
{
    _cur_tok = std::make_unique<PairTokenId>(get_tok_f());
}

inline void Parser::set_next_tok()
{
    _next_tok = std::make_unique<PairTokenId>(get_tok_f());
}
// inline PairTokenId &Parser::popout() { return _tokens_l.at(_pos++); }

inline void Parser::match(const ScriptToken &_tok)
{
    set_current_tok();
    if (_verb) {
        std::cout << "match"
                  << "\t";
        std::cout << _tok << "\t";
        std::cout << _cur_tok->token << "\n";
    }
    if (_cur_tok->token != _tok) {
#if __cpp_lib_format
        throw ParsingError(
            std::format("Parsing error on line {}\n", _cur_line));
#else
        throw ParsingError("Parsing error on line " +
                           std::to_string(_cur_line) + "\n");
#endif
    }
}

inline void Parser::match_cur(const ScriptToken &_tok)
{
    if (_verb) {
        std::cout << "match"
                  << "\t";
        std::cout << _tok << "\t";
        std::cout << _next_tok->token << "\n";
    }
    if (_next_tok->token != _tok) {
#if __cpp_lib_format
        throw ParsingError(
            std::format("Parsing error on line {}\n", _cur_line));
#else
        throw ParsingError("Parsing error on line " +
                           std::to_string(_cur_line) + "\n");
#endif
    }
}

inline ScriptAst Parser::script()
{
    ScriptAst ast;

    if (_verb) {
        std::cout << "script\t";
    }

    std::shared_ptr<StmtAst> cur_stmt;
    cur_stmt = std::make_shared<StmtAst>(stmt());
    ast.stmt = cur_stmt;
    cur_stmt = cur_stmt->next_stmt;
    while (_cur_tok->token != st::EXIT) {
        // exchange cur_stmt
        cur_stmt = std::make_shared<StmtAst>(stmt());
        cur_stmt = cur_stmt->next_stmt;
    }
    ExitAst exit;
    cur_stmt = std::make_shared<StmtAst>();
    // ast.ex = std::make_unique<ExitAst>();
    return ast;
}

StmtAst Parser::stmt()
{
    set_current_tok();
    while (_cur_tok->token == st::NEW_LINE) set_current_tok();
    StmtAst main_stmt;

    if (_verb) {
        std::cout << "stmt\t" << _cur_tok->token << "\n";
    }

    switch (_cur_tok->token) {
        case st::PERSONA: {
            match(st::VAR);
            PersonaAst persona_stmt(_cur_tok->id);
            match(st::BRACE_OP);
            persona_stmt.inside = std::make_unique<InsideAst>(inside());
            match(st::BRACE_CL);
            main_stmt.expr = std::make_unique<BasicAst>(persona_stmt);
            break;
        }
        case st::VAR: {
            const std::string p_name = _cur_tok->id;
            match(st::DOT);
            match(st::VAR);
            const std::string memb_name = _cur_tok->id;
            match(st::SIGN_EQ);
            match(st::TEXT_LINE);
            PersonaVarAst pers_var(std::move(p_name), std::move(memb_name),
                                   std::move(_cur_tok->id));
            main_stmt.expr = std::make_unique<BasicAst>(pers_var);
            break;
        }
        case st::BG: {
            match(st::SIGN_EQ);
            match(st::TEXT_LINE);
            BackFileAst bg(_cur_tok->id);
            main_stmt.expr = std::make_unique<BasicAst>(bg);
            break;
        }
        case st::FG: {
            match(st::SIGN_EQ);
            set_current_tok();
            if (_cur_tok->token == st::VAR) {
                const std::string p_name(std::move(_cur_tok->id));
                set_current_tok();
                if (_cur_tok->token == st::DOT) {
                    match(st::VAR);
                    ForePersonaAst fg_pers(std::move(p_name),
                                           std::move(_cur_tok->id));
                    main_stmt.expr = std::make_unique<BasicAst>(fg_pers);
                    break;
                }
                match_cur(st::NEW_LINE);
                ForePersonaAst fg_pers(std::move(p_name), "foreground");
                main_stmt.expr = std::make_unique<BasicAst>(fg_pers);
                return main_stmt;
            }
            match(st::TEXT_LINE);
            ForeFileAst fg_file(std::move(_cur_tok->id));
            main_stmt.expr = std::make_unique<BasicAst>(fg_file);
            break;
        }
        case st::TEXT_TYPE: {
            set_current_tok();
            if (_cur_tok->token == st::SIGN_EQ) {
                match(st::TEXT_LINE);
                TextFileAst txt_file(std::move(_cur_tok->id));
                main_stmt.expr = std::make_unique<BasicAst>(txt_file);
            } else {
                match_cur(st::TEXT_LINE);
                TextLineAst txt_data(std::move(_cur_tok->id));
                main_stmt.expr = std::make_unique<BasicAst>(txt_data);
            }
            break;
        }
        case st::EXIT: {
            main_stmt.expr = std::make_unique<ExitAst>();
            return main_stmt;
        }
        default:
            throw ParsingError("Stmt error on line" +
                               std::to_string(_cur_line));
    }

    match(st::NEW_LINE);
    _cur_line++;
    return main_stmt;
}

InsideAst Parser::inside()
{
    InsideAst main_inside;
    set_current_tok();
    if (_cur_tok->token == st::NEW_LINE) set_current_tok();
    main_inside.memb_type = std::make_unique<InsTypeAst>(type());
    match(st::SIGN_EQ);
    match(st::TEXT_LINE);
    main_inside.memb_type->str_param = std::move(_cur_tok->id);

    std::shared_ptr<InsideAst> cur_inside = main_inside.next;
    set_current_tok();
    while (_cur_tok->token == st::COMMA) {
        set_current_tok();
        if (_cur_tok->token == st::NEW_LINE) set_current_tok();
        cur_inside->memb_type = std::make_unique<InsTypeAst>(type());
        match(st::SIGN_EQ);
        match(st::TEXT_LINE);
        cur_inside->memb_type->str_param = std::move(_cur_tok->id);

        cur_inside = cur_inside->next;
        set_current_tok();
    }
    
    return main_inside;
}
// changes : now BG can't be tied to PERSONA, but different VARs as subs can
// also it checks not next, but current token
inline InsTypeAst Parser::type()
{
    if (_verb) {
        std::cout << _cur_tok->token << "\t";
    }
    if (_cur_tok->token == st::FG || _cur_tok->token == st::PATH ||
        _cur_tok->token == st::NAME || _cur_tok->token == st::VAR) {
        return InsTypeAst(std::move(*_cur_tok));
    }
#if __cpp_lib_format
    throw ParsingError(
        std::format("Error in 'persona' at line {}:\n-Only 'background', "
                    "'foreground', 'path' and variable allowed inside\n",
                    _cur_line));
#else
    throw ParsingError("Error in 'persona' at line " +
                       std::to_string(_cur_line) +
                       ":\n-Only 'background', 'foreground', 'path' and "
                       "variable are allowed inside\n");
#endif
}

void Parser::set_input(func_type get_token_function)
{
    get_tok_f = get_token_function;
    _cur_line = 0;
}

ScriptAst Parser::run(bool verbose)
{
    _verb = verbose;
    return script();
}

}  // namespace vino
