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

namespace vino {

void SemanticAnalyzer::set_symbol_table(
    const SymbolTableEnv& symb_table_env)
{
    _env = symb_table_env;
}

void SemanticAnalyzer::set_symbol_table(SymbolTableEnv&& symb_table_env)
{
    _env = symb_table_env;
}

void SemanticAnalyzer::set_ast(const ScriptAst& ast)
{
    _ast = ast;
}

void SemanticAnalyzer::set_ast(ScriptAst&& ast)
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

    Visitor anal_visitor(this->_env);
    anal_visitor.analyze(&_ast);

    _verbose = false;
}

}  // namespace vino