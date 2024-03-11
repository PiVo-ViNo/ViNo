
#include "AST.h"

#include <filesystem>
#include <iostream>
#include <memory>

#include "custom_errors.h"

namespace vino {

namespace fs = std::filesystem;

void Visitor::analyze(ScriptAst* mn_script_ptr) const
{
    // Q: will all be deleted, because smart ptr call destructor?
    // A: no, because it's shared and it's 2nd ownership
    std::shared_ptr<StmtAst> cur_stmt_ptr = mn_script_ptr->stmt;
    cur_stmt_ptr->accept(*this);

    while (cur_stmt_ptr->next_stmt != nullptr) {
        cur_stmt_ptr = cur_stmt_ptr->next_stmt;
        cur_stmt_ptr->accept(*this);
    }
}

void Visitor::analyze(StmtAst* mn_stmt_ptr) const
{
    if (mn_stmt_ptr->expr == nullptr) {
        throw SemanticError(
            "Error: nullptr as StmtAst.expr, wrong behaviour of Parser;\n");
    }
    mn_stmt_ptr->expr->accept(*this);
}

void Visitor::analyze(PersonaAst* mn_persona_ptr) const
{
    // add to Env (if there is a persona with the same name: new link in Env)
    Persona persona = env_reference.add_persona(mn_persona_ptr->p_id);
    // if possible, don't use recursion, because insides could be very long,
    // use stack/cycle
    if (mn_persona_ptr->inside == nullptr) {
        std::cout << "Warning: empty PersonaAst;\n";
        return;
    }

    std::shared_ptr<InsideAst> cur_inside_ptr = mn_persona_ptr->inside;

    // analyze(cur_inside_ptr.get());
    if (cur_inside_ptr->memb_type == nullptr &&
        cur_inside_ptr->next != nullptr) {
        throw SemanticError("Error: empty InsideAst node;\n");
    }
    analyze(cur_inside_ptr->memb_type.get());

    while (cur_inside_ptr->next != nullptr) {
        cur_inside_ptr = cur_inside_ptr->next;

        // analyze(cur_inside_ptr.get());
        if (cur_inside_ptr->memb_type == nullptr &&
            cur_inside_ptr->next != nullptr) {
            throw SemanticError("Error: empty InsideAst node;\n");
        }
        analyze(cur_inside_ptr->memb_type.get());
    }
}

void Visitor::analyze(InsideAst* mn_inside_ptr) const
{
    if (mn_inside_ptr->memb_type == nullptr && mn_inside_ptr->next != nullptr) {
        throw SemanticError("Error: empty InsideAst node;\n");
    }
    analyze(mn_inside_ptr->memb_type.get());
}

void Visitor::true_analyze(InsTypeAst* mn_ins_type_ptr, Persona& persona) const
{
    using st = ScriptToken;

    switch (mn_ins_type_ptr->lexem.token) {
        case st::PATH: {
            fs::path temp_path(mn_ins_type_ptr->str_param);
            if (fs::exists(temp_path) && fs::is_directory(temp_path)) {
                if (persona.set_path(mn_ins_type_ptr->str_param)) {
                    return;
                }
                throw SemanticError(
                    "Error: double initializing Path in Persona;\n");
            }
            throw SemanticError("Error: path in persona: " +
                                temp_path.string() + " is not a directory;\n");
        }
        case st::VAR: {
            if (persona.add_fg_var(mn_ins_type_ptr->lexem.id,
                                   mn_ins_type_ptr->str_param)) {
                fs::path temp_path(mn_ins_type_ptr->str_param);
                if (fs::exists(temp_path) && temp_path.extension() == ".png") {
                    return;
                }
                throw SemanticError("Error: " + mn_ins_type_ptr->str_param +
                                    " doesn't exist or not a .png;\n");
            }
            throw SemanticError("Error: double initializing " +
                                mn_ins_type_ptr->lexem.id + " in Persona;\n");
        }
        case st::NAME: {
            if (persona.set_name(mn_ins_type_ptr->str_param)) {
                return;
            }
            throw SemanticError(
                "Error: double initializing name in Persona;\n");
        }
        case st::FG: {
            if (persona.set_main_fg(mn_ins_type_ptr->str_param)) {
                fs::path temp_path(mn_ins_type_ptr->str_param);
                if (fs::exists(temp_path) && temp_path.extension() == ".png") {
                    return;
                }
                throw SemanticError("Error: " + mn_ins_type_ptr->str_param +
                                    " doesn't exist or not a .png;\n");
            }
            throw SemanticError(
                "Error: double initializing foreground in Persona;\n");
        }
        default:
            throw SemanticError(
                "Not PATH, VAR, NAME or FG token as Persona member, check your "
                "Parser for bugs, dude;\n");
    }
}

void Visitor::analyze(InsTypeAst* mn_ins_type_ptr) const
{
    throw SemanticError("Error: analyze(InsTypeAst) must never be called;\n");
}

void Visitor::analyze(PersonaVarAst* mn_p_var_ptr) const
{
    // param must be valid path
    fs::path temp_path(mn_p_var_ptr->param);
    if (!fs::exists(temp_path) && temp_path.extension() != ".png") {
        throw SemanticError("Error: " + mn_p_var_ptr->param +
                            " doesn't exist or not a .png;\n");
    }
    // persona must be initialized
    if (!env_reference.add_info_to(mn_p_var_ptr->p_id, mn_p_var_ptr->memb_id,
                                   mn_p_var_ptr->param)) {
        throw SemanticError("Error: persona " + mn_p_var_ptr->p_id +
                            " wasn't initialized;\n");
    }
}

void Visitor::analyze(BackFileAst* mn_bg_ptr) const
{
    fs::path temp_path(mn_bg_ptr->path_bg);
    if (!fs::exists(temp_path) && temp_path.extension() != ".png") {
        throw SemanticError("Error: " + mn_bg_ptr->path_bg +
                            " doesn't exist or not a .png;\n");
    }
}

void Visitor::analyze(ForeFileAst* mn_fg_f_ptr) const
{
    fs::path temp_path(mn_fg_f_ptr->path_fg);
    if (!fs::exists(temp_path) && temp_path.extension() != ".png") {
        throw SemanticError("Error: " + mn_fg_f_ptr->path_fg +
                            " doesn't exist or not a .png;\n");
    }
}

void Visitor::analyze(ForePersonaAst* fg_pers_ptr) const
{
    if (!env_reference.exists(fg_pers_ptr->p_id)) {
        throw SemanticError("Error: persona " + fg_pers_ptr->p_id +
                            " wasn't initialized;\n");
    }
    std::string fg_path = env_reference.get_persona(fg_pers_ptr->p_id)
                              .get_fg(fg_pers_ptr->memb_name);
    if (fg_path.empty()) {
        throw SemanticError("Error: member " + fg_pers_ptr->memb_name +
                            " of persona " + fg_pers_ptr->p_id +
                            " wasn't initialized;\n");
    }
    // const propagation?
}

void Visitor::analyze(TextLineAst* txt_line_ptr) const
{
    return;
}

void Visitor::analyze(TextFileAst* txt_file_ptr) const
{
    fs::path temp_path(txt_file_ptr->path_txt);
    if (!fs::exists(temp_path) && temp_path.extension() != ".txt") {
        throw SemanticError("Error: " + txt_file_ptr->path_txt +
                            " doesn't exist or not a .txt;\n");
    }
}

void Visitor::analyze(ExitAst* cur_ast) const
{
    return;
}

}  // namespace vino