/*
 * SemanticAnalyzer.cpp
 *
 * Created on Sun Mar 10 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#include "SemanticAnalyzer.h"

#include <iostream>
#include <memory>


namespace vino {

inline void SemanticAnalyzer::set_symbol_table(
    const SymbolTableEnv& symb_table_env)
{
    _env = symb_table_env;
}

inline void SemanticAnalyzer::set_symbol_table(SymbolTableEnv&& symb_table_env)
{
    _env = symb_table_env;
}

inline void SemanticAnalyzer::set_ast(const ScriptAst& ast)
{
    _ast = ast;
}

inline void SemanticAnalyzer::set_ast(ScriptAst&& ast)
{
    _ast = ast;
}

void SemanticAnalyzer::run_analysis(bool verbose)
{
    if (_ast.stmt == nullptr) {
        std::cout << "Input AST was empty: correct semantics for sure x_x\n";
        return;
    }

    _verbose = verbose;

    std::shared_ptr<StmtAst> cur_stmt = _ast.stmt;
    cur_stmt->analyze();
    while (cur_stmt->next_stmt != nullptr) {
        cur_stmt = cur_stmt->next_stmt;
        cur_stmt->analyze();
    }

    _verbose = false;
}

}  // namespace vino