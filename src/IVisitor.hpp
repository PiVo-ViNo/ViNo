#pragma once

#include "SymbolTable.hpp"

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

    virtual void visit_exit() const = 0;
    virtual void visit_script(const ScriptAst*) const = 0;
    virtual void visit_stmt(const StmtAst*) const = 0;
    virtual void visit_persona(const PersonaAst*) const = 0;
    virtual void visit_ins_type(const InsTypeAst*, Persona&) const = 0;
    virtual void visit_persona_var(const PersonaVarAst*) const = 0;
    virtual void visit_bg_file(const BackFileAst*) const = 0;
    virtual void visit_fg_file(const ForeFileAst*) const = 0;
    virtual void visit_fg_persona(const ForePersonaAst*) const = 0;
    virtual void visit_txt_line() const = 0;
    virtual void visit_txt_file(const TextFileAst*) const = 0;

    void error() const;

protected:
    // referece instead of ptr, because no null possible (and must be)
    SymbolTableEnv& _env_reference;
    bool            _verb;
};
}  // namespace vino