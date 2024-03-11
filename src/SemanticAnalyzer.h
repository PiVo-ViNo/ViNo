/*
 * SemanticAnalyzer.h
 *
 * Created on Sun Mar 10 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#pragma once

#include <filesystem>

#include "AST.h"
#include "SymbolTable.h"
#include "custom_errors.h"

namespace vino {

class SemanticAnalyzer {
public:
    SemanticAnalyzer(SymbolTableEnv& symb_table, const ScriptAst& ast) :
        _env(symb_table), _ast(ast)
    {
    }

    SemanticAnalyzer(SymbolTableEnv&& symb_table, ScriptAst&& ast) :
        _env(symb_table), _ast(ast)
    {
    }

    void set_symbol_table(const SymbolTableEnv&);
    void set_symbol_table(SymbolTableEnv&&);

    void set_ast(const ScriptAst&);
    void set_ast(ScriptAst&&);

    void run_analysis(bool verbose = false);

private:
    SymbolTableEnv& _env; // reference&, because SemAnal exchange info with it
    ScriptAst       _ast; // SemAnal take whole AST
    bool            _verbose = false;
};

}  // namespace vino