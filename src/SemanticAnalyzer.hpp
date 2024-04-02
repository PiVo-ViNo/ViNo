/*
 * SemanticAnalyzer.hpp
 *
 * Created on Sun Mar 10 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#pragma once

#include "custom_errors.hpp"
#include "AST.hpp"
#include "SymbolTable.hpp"
#include <filesystem>

namespace vino {

class SemanticAnalyzer {
public:
    SemanticAnalyzer(SymbolTableEnv& symb_table, const ScriptAst& ast) :
        _env(symb_table), _ast(&ast), _moved_ast(false)
    {
    }

    SemanticAnalyzer(SymbolTableEnv&& symb_table, ScriptAst&& ast) :
        _env(symb_table), _ast(&ast), _moved_ast(true)
    {
    }

    ~SemanticAnalyzer()
    {
        if (!_moved_ast) {
            _ast.release();
        }
    }

    void set_symbol_table(const SymbolTableEnv&);
    void set_symbol_table(SymbolTableEnv&&);

    // void set_ast(const ScriptAst&);
    // void set_ast(ScriptAst&&);

    void run(bool verbose = false);

private:
    SymbolTableEnv& _env;  // reference&, because SemAnal exchange info with it
    std::unique_ptr<const ScriptAst> _ast;  // SemAnal take whole AST
    bool                             _verbose = false;
    bool                             _moved_ast;
};

}  // namespace vino
