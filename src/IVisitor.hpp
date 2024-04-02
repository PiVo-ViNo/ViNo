#pragma once

#include "SymbolTable.hpp"
#include "custom_errors.hpp"

namespace vino {

struct ExitAst;
struct StmtAst;
struct ScriptAst;
struct InsTypeAst;
struct InsideAst;
struct PersonaAst;
struct PersonaVarAst;
struct BackFileAst;
struct ForeFileAst;
struct ForePersonaAst;
struct TextLineAst;
struct TextFileAst;

/**
    @brief Visitor pattern-like interface
    @details Double dispatch method
    @param env_reference
*/
class IVisitor {
public:
    explicit IVisitor(SymbolTableEnv& symb_table_env, bool verb) :
        _env_reference(symb_table_env), _verb(verb)
    {
    }

    virtual ~IVisitor() = default;

    virtual void visit_exit() = 0;
    virtual void visit_script(const ScriptAst*) = 0;
    virtual void visit_stmt(const StmtAst*) = 0;
    virtual void visit_persona(const PersonaAst*) = 0;
    virtual void visit_ins_type(const InsTypeAst*, Persona&) = 0;
    virtual void visit_persona_var(const PersonaVarAst*) = 0;
    virtual void visit_bg_file(const BackFileAst*) = 0;
    virtual void visit_fg_file(const ForeFileAst*) = 0;
    virtual void visit_fg_persona(const ForePersonaAst*) = 0;
    virtual void visit_txt_line(const TextLineAst*) = 0;
    virtual void visit_txt_file(const TextFileAst*) = 0;

    void error() const
    {
        throw SemanticError("Program error: calling deprecated visitor method\n");
    }

protected:
    // referece instead of ptr, because no null possible (and must be)
    SymbolTableEnv& _env_reference;
    bool            _verb;
};
}  // namespace vino