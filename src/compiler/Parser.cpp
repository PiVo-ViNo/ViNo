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

#include "Parser.hpp"

#include "AST.hpp"
#include "TokenEnum.hpp"
#include "custom_errors.hpp"
#include <iostream>
#include <memory>

namespace vino {

using st = ScriptToken;

void Parser::set_current_tok()
{
    _cur_tok = std::make_unique<PairTokenId>(get_tok_f());
}

void Parser::match(const ScriptToken &tok)
{
    set_current_tok();
    if (_verb) {
        std::cout << "match\t";
        std::cout << tok << "\t";
        std::cout << _cur_tok->token << "\n";
    }
    if (_cur_tok->token != tok) {
        throw ParsingError(
                "Parsing error on line " + std::to_string(_cur_line) + "\n");
    }
}

void Parser::match_cur(const ScriptToken &tok)
{
    if (_verb) {
        std::cout << "match cur\t";
        std::cout << tok << "\t";
        std::cout << _cur_tok->token << "\n";
    }
    if (_cur_tok->token != tok) {
        throw ParsingError(
                "Parsing error on line " + std::to_string(_cur_line) + "\n");
    }
}

ScriptAst Parser::script()
{
    ScriptAst ast;

    if (_verb) {
        std::cout << "script\t";
    }

    ast.stmt = std::make_unique<StmtAst>(stmt());
    StmtAst *ptrStmtCur = ast.stmt.get();

    while (_cur_tok->token != st::EXIT) {
        // exchange cur_stmt
        ptrStmtCur->next_stmt = std::make_unique<StmtAst>(stmt());
        ptrStmtCur = ptrStmtCur->next_stmt.get();
    }
    return ast;
}

StmtAst Parser::stmt()
{
    set_current_tok();
    while (_cur_tok->token == st::NEW_LINE) {
        set_current_tok();
    }

    StmtAst main_stmt;

    if (_verb) {
        std::cout << "stmt\t" << _cur_tok->token << "\n";
    }

    switch (_cur_tok->token) {
        case st::PERSONA: {
            match(st::VAR);
            auto ptr_persona_stmt = std::make_unique<PersonaAst>(_cur_tok->id);
            if (_verb) {
                // std::cout << "\nPersona&: " << ptr_persona_stmt << '\n';
            }
            match(st::BRACE_OP);
            ptr_persona_stmt->inside = std::make_unique<InsideAst>(inside());
            match_cur(st::BRACE_CL);
            main_stmt.expr = std::move(ptr_persona_stmt);
            if (_verb) {
                // std::cout << "\nPersona&: " << main_stmt.expr << '\n';
            }
            break;
        }
        case st::VAR: {
            const std::string p_name = _cur_tok->id;
            match(st::DOT);
            match(st::VAR);
            const std::string memb_name = _cur_tok->id;
            match(st::SIGN_EQ);
            match(st::TEXT_LINE);
            main_stmt.expr = std::make_unique<PersonaVarAst>(std::move(p_name),
                    std::move(memb_name), std::move(_cur_tok->id));
            break;
        }
        case st::BG: {
            match(st::SIGN_EQ);
            match(st::TEXT_LINE);
            main_stmt.expr = std::make_unique<BackFileAst>(_cur_tok->id);
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
                    main_stmt.expr = std::make_unique<ForePersonaAst>(
                            std::move(p_name), std::move(_cur_tok->id));
                    break;
                }
                match_cur(st::NEW_LINE);
                main_stmt.expr = std::make_unique<ForePersonaAst>(
                        std::move(p_name), "foreground");
                return main_stmt;
            }
            match_cur(st::TEXT_LINE);
            main_stmt.expr =
                    std::make_unique<ForeFileAst>(std::move(_cur_tok->id));
            break;
        }
        case st::TEXT_TYPE: {
            set_current_tok();
            if (_cur_tok->token == st::SIGN_EQ) {
                match(st::TEXT_LINE);
                main_stmt.expr =
                        std::make_unique<TextFileAst>(std::move(_cur_tok->id));
            } else {
                match_cur(st::TEXT_LINE);
                main_stmt.expr =
                        std::make_unique<TextLineAst>(std::move(_cur_tok->id));
            }
            break;
        }
        case st::EXIT: {
            main_stmt.expr = std::make_unique<ExitAst>();
            return main_stmt;
        }
        default:
            throw ParsingError(
                    "Stmt error on line" + std::to_string(_cur_line));
    }

    match(st::NEW_LINE);
    _cur_line++;
    return main_stmt;
}

InsideAst Parser::inside()
{
    InsideAst main_inside;
    set_current_tok();
    if (_cur_tok->token == st::BRACE_CL) {
        return main_inside;
    }
    if (_cur_tok->token == st::NEW_LINE) {
        _cur_line++;
        set_current_tok();
    }

    main_inside.memb_type = std::make_unique<InsTypeAst>(type());

    match(st::SIGN_EQ);
    match(st::TEXT_LINE);
    main_inside.memb_type->str_param = std::move(_cur_tok->id);

    InsideAst *ptrInsideCur = &main_inside;

    set_current_tok();
    while (_cur_tok->token == st::COMMA) {
        ptrInsideCur->next = std::make_unique<InsideAst>();

        set_current_tok();
        while (_cur_tok->token == st::NEW_LINE) {  // maybe add counter?
            _cur_line++;
            set_current_tok();
        }
        ptrInsideCur->next->memb_type = std::make_unique<InsTypeAst>(type());
        match(st::SIGN_EQ);
        match(st::TEXT_LINE);
        ptrInsideCur->next->memb_type->str_param = std::move(_cur_tok->id);

        ptrInsideCur = ptrInsideCur->next.get();
        set_current_tok();
    }

    return main_inside;
}
// changes : now BG can't be tied to PERSONA, but different VARs as subs can
// also it checks not next, but current token
InsTypeAst Parser::type()
{
    if (_verb) {
        std::cout << _cur_tok->token << "\t";
    }
    if (_cur_tok->token == st::FG || _cur_tok->token == st::PATH
            || _cur_tok->token == st::NAME || _cur_tok->token == st::VAR)
    {
        return InsTypeAst(*_cur_tok);
    }
    throw ParsingError("Error in 'persona' at line " +
                       std::to_string(_cur_line) +
                       ":\n-Only 'name', 'foreground', 'path' and "
                       "variable are allowed inside\n");
}

void Parser::set_input(func_type get_token_function)
{
    get_tok_f = get_token_function;
    _cur_line = 1;
}

ScriptAst Parser::run(bool verbose)
{
    _verb = verbose;
    ScriptAst script = this->script();
    _verb = false;
    return script;
}

Parser::Parser(func_type get_token_function) :
    get_tok_f(std::move(get_token_function))
{
}

Parser::Parser(Parser &&_p) :
    _cur_tok(std::move(_p._cur_tok)), get_tok_f(std::move(_p.get_tok_f))
{
}

}  // namespace vino
