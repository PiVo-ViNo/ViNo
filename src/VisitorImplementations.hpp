#pragma once

#include "IVisitor.hpp"
#include "custom_errors.hpp"

#include <ostream>
#include <filesystem>
#include <set>
#include <stdexcept>

namespace vino {

namespace fs = std::filesystem;

class SemanticVisitor : public IVisitor {
public:
    SemanticVisitor(SymbolTableEnv& symb_table_env, bool verb) :
        IVisitor(symb_table_env, verb)
    {
    }

    void visit_exit() override;
    void visit_script(const ScriptAst*) override;
    void visit_stmt(const StmtAst*) override;
    void visit_persona(const PersonaAst*) override;
    void visit_ins_type(const InsTypeAst*, Persona&) override;
    void visit_persona_var(const PersonaVarAst*) override;
    void visit_bg_file(const BackFileAst*) override;
    void visit_fg_file(const ForeFileAst*) override;
    void visit_fg_persona(const ForePersonaAst*) override;
    void visit_txt_line(const TextLineAst*) override;
    void visit_txt_file(const TextFileAst*) override;

private:
};

class CodeGenVisitor : public IVisitor {
public:
    CodeGenVisitor(std::ostream& out_file, SymbolTableEnv& symb_table_env,
                   bool verb) :
        IVisitor(symb_table_env, verb), _out_bin_file(out_file)
    {
        if (_out_bin_file.bad()) {
            throw std::runtime_error("Bad std::ostream in CodeGenVisitor");
        }
    }

    void visit_exit() override;
    void visit_script(const ScriptAst*) override;
    void visit_stmt(const StmtAst*) override;
    void visit_persona(const PersonaAst*) override;
    void visit_ins_type(const InsTypeAst*, Persona&) override;
    void visit_persona_var(const PersonaVarAst*) override;
    void visit_bg_file(const BackFileAst*) override;
    void visit_fg_file(const ForeFileAst*) override;
    void visit_fg_persona(const ForePersonaAst*) override;
    void visit_txt_line(const TextLineAst*) override;
    void visit_txt_file(const TextFileAst*) override;

    [[nodiscard]] const std::set<fs::path>& get_copyset() const;

private:
    std::ostream& _out_bin_file;
    /// Set that fills when visit_script
    std::set<fs::path> _res_paths{};
};

}  // namespace vino