#include "AST.h"

#include <filesystem>
#include <iostream>
#include <memory>

#include "custom_errors.h"

namespace vino {

namespace fs = std::filesystem;

void Visitor::analyze(const ScriptAst* mn_script_ptr) const
{
    // Q: will all be deleted, because smart ptr call destructor?
    // A: no, because it's shared and it's 2nd ownership
    // T: now we use basic pointer, hooray! And it won't cause any leak!
    const StmtAst* ptrCurStmt = mn_script_ptr->stmt.get();
    ptrCurStmt->accept(*this);

    while (ptrCurStmt->next_stmt != nullptr) {
        ptrCurStmt = ptrCurStmt->next_stmt.get();
        ptrCurStmt->accept(*this);
    }
}

void Visitor::analyze(const StmtAst* mn_stmt_ptr) const
{
    if (mn_stmt_ptr->expr == nullptr) {
        throw SemanticError(
            "Error: nullptr as StmtAst.expr, wrong behaviour of Parser;\n");
    }
    mn_stmt_ptr->expr->accept(*this);
}

void Visitor::analyze(const PersonaAst* mn_persona_ptr) const
{
    // add to Env (if there is a persona with the same name: new link in Env)
    Persona& persona = env_reference.add_persona(mn_persona_ptr->p_id);
    // if possible, don't use recursion, because insides could be very long,
    // use stack/cycle
    if (mn_persona_ptr->inside == nullptr) {
        std::cout << "Warning: empty PersonaAst;\n";
        return;
    }

    if (verbal) {
        std::cout << "persona\t" << mn_persona_ptr->p_id << '\n' << std::flush;
    }

    const InsideAst* ptrCurInside = mn_persona_ptr->inside.get();

    if (ptrCurInside->memb_type == nullptr && ptrCurInside->next != nullptr) {
        throw SemanticError("Error: empty InsideAst node;\n");
    }
    true_analyze(ptrCurInside->memb_type.get(), persona);

    while (ptrCurInside->next != nullptr) {
        ptrCurInside = ptrCurInside->next.get();
        if (ptrCurInside->memb_type == nullptr && ptrCurInside->next != nullptr)
        {
            throw SemanticError("Error: empty InsideAst node;\n");
        }
        true_analyze(ptrCurInside->memb_type.get(), persona);
    }
}

void Visitor::analyze(const InsideAst* mn_inside_ptr) const
{
    if (mn_inside_ptr->memb_type == nullptr && mn_inside_ptr->next != nullptr) {
        throw SemanticError("Error: empty InsideAst node;\n");
    }
    analyze(mn_inside_ptr->memb_type.get());
}

void Visitor::true_analyze(const InsTypeAst* mn_ins_type_ptr,
                           Persona&          persona) const
{
    using st = ScriptToken;

    switch (mn_ins_type_ptr->lexem.token) {
        case st::PATH: {
            if (verbal) {
                std::cout << "\tpath\t" << mn_ins_type_ptr->str_param << '\n'
                          << std::flush;
            }
            fs::path temp_path(mn_ins_type_ptr->str_param);
            if (fs::exists(temp_path) && fs::is_directory(temp_path)) {
                if (persona.set_path(mn_ins_type_ptr->str_param)) {
                    return;
                }
                throw SemanticError(
                    "Error: double initializing Path in Persona;\n");
            }
            throw SemanticError("Error: path in persona: " + temp_path.string()
                                + " is not a directory;\n");
        }
        case st::VAR: {
            if (verbal) {
                std::cout << "\tvar\t" << mn_ins_type_ptr->lexem.id << '\n'
                          << std::flush;
            }
            if (persona.add_fg_var(mn_ins_type_ptr->lexem.id,
                                   mn_ins_type_ptr->str_param))
            {
                fs::path temp_path(mn_ins_type_ptr->str_param);
                if (fs::exists(temp_path) && temp_path.extension() == ".png") {
                    return;
                }
                throw SemanticError("Error: " + mn_ins_type_ptr->str_param
                                    + " doesn't exist or not a .png;\n");
            }
            throw SemanticError("Error: double initializing "
                                + mn_ins_type_ptr->lexem.id + " in Persona;\n");
        }
        case st::NAME: {
            std::cout << "\tname\t" << mn_ins_type_ptr->str_param << '\n'
                      << std::flush;
            if (persona.set_name(mn_ins_type_ptr->str_param)) {
                return;
            }
            throw SemanticError(
                "Error: double initializing name in Persona;\n");
        }
        case st::FG: {
            std::cout << "\tfg\t" << std::flush;
            if (persona.set_main_fg(mn_ins_type_ptr->str_param)) {
                if (verbal) {
                    std::cout << persona.get_main_fg() << std::endl;
                }
                fs::path temp_path(mn_ins_type_ptr->str_param);
                if (fs::exists(temp_path) && temp_path.extension() == ".png") {
                    return;
                }
                throw SemanticError("Error: " + mn_ins_type_ptr->str_param
                                    + " doesn't exist or not a .png;\n");
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

void Visitor::analyze(const InsTypeAst* mn_ins_type_ptr) const
{
    throw SemanticError("Error: analyze(InsTypeAst) must never be called;\n");
}

void Visitor::analyze(const PersonaVarAst* mn_p_var_ptr) const
{
    if (verbal) {
        std::cout << "personavar\n" << std::flush;
    }
    // param must be valid path
    fs::path temp_path(mn_p_var_ptr->param);
    if (!fs::exists(temp_path) && temp_path.extension() != ".png") {
        throw SemanticError("Error: " + mn_p_var_ptr->param
                            + " doesn't exist or not a .png;\n");
    }
    // persona must be initialized
    if (!env_reference.add_info_to(mn_p_var_ptr->p_id, mn_p_var_ptr->memb_id,
                                   mn_p_var_ptr->param))
    {
        throw SemanticError("Error: persona " + mn_p_var_ptr->p_id
                            + " wasn't initialized;\n");
    }
}

void Visitor::analyze(const BackFileAst* mn_bg_ptr) const
{
    if (verbal) {
        std::cout << "bgfile\n" << std::flush;
    }
    fs::path temp_path(mn_bg_ptr->path_bg);
    if (!fs::exists(temp_path) && temp_path.extension() != ".png") {
        throw SemanticError("Error: " + temp_path.string()
                            + " doesn't exist or not a .png;\n");
    }
}

void Visitor::analyze(const ForeFileAst* mn_fg_f_ptr) const
{
    if (verbal) {
        std::cout << "fgfile\n" << std::flush;
    }
    fs::path temp_path(mn_fg_f_ptr->path_fg);
    if (!fs::exists(temp_path) && temp_path.extension() != ".png") {
        throw SemanticError("Error: " + temp_path.string()
                            + " doesn't exist or not a .png;\n");
    }
}

void Visitor::analyze(const ForePersonaAst* fg_pers_ptr) const
{
    if (verbal) {
        std::cout << "fgpersona\t" << fg_pers_ptr->p_id << std::endl;
    }
    // Persona with such id must have been initialized in Env before
    if (!env_reference.exists(fg_pers_ptr->p_id)) {
        throw SemanticError("Error: persona " + fg_pers_ptr->p_id
                            + " wasn't initialized;\n");
    }
    std::string fg_path;
    if (fg_pers_ptr->memb_name == "foreground") {
        fg_path = env_reference.get_persona(fg_pers_ptr->p_id).get_main_fg();
    } else {
        fg_path = env_reference.get_persona(fg_pers_ptr->p_id)
                      .get_fg(fg_pers_ptr->memb_name);
    }
    if (fg_path.empty()) {
        throw SemanticError("Error: member " + fg_pers_ptr->memb_name
                            + " of persona " + fg_pers_ptr->p_id
                            + " wasn't initialized;\n");
    }
    // const propagation?
}

void Visitor::analyze(const TextLineAst* txt_line_ptr) const
{
    if (verbal) {
        std::cout << "txtline\n" << std::flush;
    }
    return;
}

void Visitor::analyze(const TextFileAst* txt_file_ptr) const
{
    if (verbal) {
        std::cout << "txtfile\n" << std::flush;
    }
    fs::path temp_path(txt_file_ptr->path_txt);
    if (!fs::exists(temp_path) && temp_path.extension() != ".txt") {
        throw SemanticError("Error: " + txt_file_ptr->path_txt
                            + " doesn't exist or not a .txt;\n");
    }
}

void Visitor::analyze(const ExitAst* cur_ast) const
{
    if (verbal) {
        std::cout << "exit\n" << std::flush;
    }
    return;
}

}  // namespace vino