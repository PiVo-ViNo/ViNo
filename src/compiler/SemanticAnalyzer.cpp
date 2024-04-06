/*
 * SemanticAnalyzer.cpp
 *
 * Created on Sun Mar 10 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#include "SemanticAnalyzer.hpp"

#include "VisitorImplementations.hpp"
#include <iostream>

namespace vino {

void SemanticAnalyzer::set_symbol_table(const SymbolTableEnv& symb_table_env)
{
    _env = symb_table_env;
}

void SemanticAnalyzer::set_symbol_table(SymbolTableEnv&& symb_table_env)
{
    _env = symb_table_env;
}

void SemanticAnalyzer::run(bool verbose)
{
    if (_ast->stmt == nullptr) {
        std::cout << "Input AST was empty: correct semantics for sure x_x\n";
        return;
    }

    _verb = verbose;

    SemanticVisitor anal_visitor(this->_env, _verb);
    anal_visitor.visit_script(_ast.get());

    _verb = false;
}

}  // namespace vino
