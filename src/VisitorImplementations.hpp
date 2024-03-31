#pragma once

#include "IVisitor.hpp"

namespace vino {

class SemanticVisitor : public IVisitor {
public:
    SemanticVisitor(SymbolTableEnv& symb_table_env, bool verb) :
        IVisitor(symb_table_env, verb)
    {
    }

    void visit_exit() const override;
    void visit_script(const ScriptAst*) const override;
    void visit_stmt(const StmtAst*) const override;
    void visit_persona(const PersonaAst*) const override;
    void visit_ins_type(const InsTypeAst*, Persona&) const override;
    void visit_persona_var(const PersonaVarAst*) const override;
    void visit_bg_file(const BackFileAst*) const override;
    void visit_fg_file(const ForeFileAst*) const override;
    void visit_fg_persona(const ForePersonaAst*) const override;
    void visit_txt_line() const override;
    void visit_txt_file(const TextFileAst*) const override;

private:
};

class FlatterVisitor : public IVisitor {
public:
    FlatterVisitor(SymbolTableEnv& symb_table_env, bool verb) :
        IVisitor(symb_table_env, verb)
    {
    }

    void visit_exit() const override;
    void visit_script(const ScriptAst*) const override;
    void visit_stmt(const StmtAst*) const override;
    void visit_persona(const PersonaAst*) const override;
    void visit_ins_type(const InsTypeAst*, Persona&) const override;
    void visit_persona_var(const PersonaVarAst*) const override;
    void visit_bg_file(const BackFileAst*) const override;
    void visit_fg_file(const ForeFileAst*) const override;
    void visit_fg_persona(const ForePersonaAst*) const override;
    void visit_txt_line() const override;
    void visit_txt_file(const TextFileAst*) const override;

private:
};

}  // namespace vino