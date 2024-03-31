/*
 * SemanticAnalyzer.cpp
 *
 * Created on Sun Mar 10 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#include "SemanticAnalyzer.hpp"

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

void SemanticAnalyzer::run_analysis(bool verbose)
{
    if (_ast->stmt == nullptr) {
        std::cout << "Input AST was empty: correct semantics for sure x_x\n";
        return;
    }

    _verbose = verbose;

    Visitor anal_visitor(this->_env, true);
    anal_visitor.analyze_script(_ast.get());

    _verbose = false;
}

}  // namespace vino
