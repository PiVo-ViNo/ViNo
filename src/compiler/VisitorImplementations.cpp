#include "VisitorImplementations.hpp"

#include "AST.hpp"

#include "custom_errors.hpp"
#include "stuff.hpp"
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <ostream>
#include <cstring>

namespace vino {

namespace fs = std::filesystem;

// SemanticVisitor ------------------------------------------------------------
// ----------------------------------------------------------------------------

bool img_at_least_one_of(const std::string& file_ext, 
        std::convertible_to<std::string> auto&&... ext)
{
    return (false || ... || insen_str_equal<char>(file_ext, ext)); 
}

bool img_has_good_extension(const std::string& file_ext)
{
    return img_at_least_one_of(file_ext, ".jpg", ".png", ".bmp",
                                ".psd", ".jpeg");
}

void SemanticVisitor::visit_exit()
{
    if (_verb) {
        std::cout << "exit\n" << std::flush;
    }
}

void SemanticVisitor::visit_script(const ScriptAst* mn_script_ptr)
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

void SemanticVisitor::visit_stmt(const StmtAst* mn_stmt_ptr)
{
    if (mn_stmt_ptr->expr == nullptr) {
        throw SemanticError(
                "Error: nullptr as StmtAst.expr, wrong behaviour of Parser;");
    }
    mn_stmt_ptr->expr->accept(*this);
}

void SemanticVisitor::visit_persona(const PersonaAst* mn_persona_ptr)
{
    // add to Env (if there is a persona with the same name: new link in Env)
    Persona& persona = _env_reference.add_persona(mn_persona_ptr->p_id);
    // if possible, don't use recursion, because insides could be very long,
    // use stack/cycle
    if (mn_persona_ptr->inside == nullptr) {
        std::cout << "Warning: empty PersonaAst;\n" << std::flush;
        return;
    }

    if (_verb) {
        std::cout << "persona\t" << mn_persona_ptr->p_id << '\n' << std::flush;
    }

    const InsideAst* ptrCurInside = mn_persona_ptr->inside.get();

    if (ptrCurInside->memb_type == nullptr && ptrCurInside->next != nullptr) {
        throw SemanticError("Error: empty InsideAst node;");
    }
    visit_ins_type(ptrCurInside->memb_type.get(), persona);

    while (ptrCurInside->next != nullptr) {
        ptrCurInside = ptrCurInside->next.get();
        if (ptrCurInside->memb_type == nullptr && ptrCurInside->next != nullptr)
        {
            throw SemanticError("Error: empty InsideAst node;");
        }
        visit_ins_type(ptrCurInside->memb_type.get(), persona);
    }
}

/// TODO: all image variables (include main FG) must lie inside persona.path
///     though their path could be absolute, check absolute path first
/// Q: what if we define persona.path after variable image? We need to 
///     make double check for persona in this case
void SemanticVisitor::visit_ins_type(
        const InsTypeAst* mn_ins_type_ptr, Persona& persona)
{
    using st = ScriptToken;

    switch (mn_ins_type_ptr->lexem.token) {
        case st::PATH: {
            if (_verb) {
                std::cout << "\tpath\t" << mn_ins_type_ptr->str_param << '\n'
                          << std::flush;
            }
            fs::path temp_path(mn_ins_type_ptr->str_param);
            if (fs::exists(temp_path) && fs::is_directory(temp_path)) {
                if (persona.set_path(mn_ins_type_ptr->str_param)) {
                    return;
                }
                throw SemanticError(
                        "Error: double initializing Path in Persona;");
            }
            throw SemanticError("Error: path in persona: " + temp_path.string()
                                + " is not a directory;");
        }
        case st::VAR: {
            if (_verb) {
                std::cout << "\tvar\t" << mn_ins_type_ptr->lexem.id << '\n'
                          << std::flush;
            }
            if (persona.add_fg_var(
                        mn_ins_type_ptr->lexem.id, mn_ins_type_ptr->str_param))
            {
                fs::path temp_path(mn_ins_type_ptr->str_param);
                if (fs::exists(temp_path) 
                    && img_has_good_extension(temp_path.extension().string())) 
                {
                    return;
                }
                throw SemanticError("Error: " + mn_ins_type_ptr->str_param
                                    + " doesn't exist or not an image;");
            }
            throw SemanticError("Error: double initializing "
                                + mn_ins_type_ptr->lexem.id + " in Persona;");
        }
        case st::NAME: {
            if (_verb) {
                std::cout << "\tname\t" << mn_ins_type_ptr->str_param << '\n'
                          << std::flush;
            }
            if (persona.set_name(mn_ins_type_ptr->str_param)) {
                return;
            }
            throw SemanticError("Error: double initializing name in Persona;");
        }
        case st::FG: {
            if (_verb) {
                std::cout << "\tfg\t" << std::flush;
            }
            if (persona.set_main_fg(mn_ins_type_ptr->str_param)) {
                if (_verb) {
                    std::cout << persona.get_main_fg() << std::endl;
                }
                fs::path temp_path(mn_ins_type_ptr->str_param);
                if (fs::exists(temp_path) 
                    && img_has_good_extension(temp_path.extension().string()))
                {
                    return;
                }
                throw SemanticError("Error: " + mn_ins_type_ptr->str_param
                                    + " doesn't exist or not an image;");
            }
            throw SemanticError(
                    "Error: double initializing foreground in Persona;");
        }
        default:
            throw SemanticError(
                    "Not PATH, VAR, NAME or FG token as Persona member, check "
                    "your Parser for bugs, dude;");
    }
}

void SemanticVisitor::visit_persona_var(const PersonaVarAst* mn_p_var_ptr)
{
    if (_verb) {
        std::cout << "personavar\n" << std::flush;
    }
    // param must be valid path
    fs::path temp_path(mn_p_var_ptr->param);
    if (!fs::exists(temp_path) 
        || !img_has_good_extension(temp_path.extension().string()))
    {
        throw SemanticError("Error: " + mn_p_var_ptr->param
                            + " doesn't exist or not an image;");
    }
    // persona must be initialized
    if (!_env_reference.add_info_to(
                mn_p_var_ptr->p_id, mn_p_var_ptr->memb_id, mn_p_var_ptr->param))
    {
        throw SemanticError("Error: persona " + mn_p_var_ptr->p_id
                            + " wasn't initialized;");
    }
}

void SemanticVisitor::visit_bg_file(const BackFileAst* mn_bg_ptr)
{
    if (_verb) {
        std::cout << "bgfile\n" << std::flush;
    }
    fs::path temp_path(mn_bg_ptr->path_bg);
    if (!fs::exists(temp_path) 
        || !img_has_good_extension(temp_path.extension().string()))
    {
        throw SemanticError("Error: " + temp_path.string()
                            + " doesn't exist or not an image;");
    }
}

void SemanticVisitor::visit_fg_file(const ForeFileAst* mn_fg_f_ptr)
{
    if (_verb) {
        std::cout << "fgfile\n" << std::flush;
    }
    fs::path temp_path(mn_fg_f_ptr->path_fg);
    if (!fs::exists(temp_path) 
        || !img_has_good_extension(temp_path.extension().string()))
    {
        throw SemanticError("Error: " + temp_path.string()
                            + " doesn't exist or not an image;");
    }
}

void SemanticVisitor::visit_fg_persona(const ForePersonaAst* fg_pers_ptr)
{
    // Persona with such id must have been initialized in Env before
    if (!_env_reference.exists(fg_pers_ptr->p_id)) {
        throw SemanticError(
                "Error: persona " + fg_pers_ptr->p_id + " wasn't initialized;");
    }
    std::string fg_path;
    if (fg_pers_ptr->memb_name == "foreground") {
        fg_path = _env_reference.get_persona(fg_pers_ptr->p_id).get_main_fg();
    } else {
        fg_path = _env_reference.get_persona(fg_pers_ptr->p_id)
                          .get_fg(fg_pers_ptr->memb_name);
    }
    if (fg_path.empty()) {
        throw SemanticError("Error: member " + fg_pers_ptr->memb_name
                            + " of persona " + fg_pers_ptr->p_id
                            + " wasn't initialized;");
    }
}

void SemanticVisitor::visit_txt_line(const TextLineAst*)
{
    if (_verb) {
        std::cout << "txtline\n" << std::flush;
    }
    return;
}

void SemanticVisitor::visit_txt_file(const TextFileAst* txt_file_ptr)
{
    if (_verb) {
        std::cout << "txtfile\n" << std::flush;
    }
    fs::path temp_path(txt_file_ptr->path_txt);
    if (!fs::exists(temp_path) || temp_path.extension() != ".txt") {
        throw SemanticError("Error: " + txt_file_ptr->path_txt
                            + " doesn't exist or not a .txt;");
    }
}

// CodeGenVisitor -------------------------------------------------------------
// ----------------------------------------------------------------------------

void CodeGenVisitor::visit_exit()
{
    if (_verb) {
        std::cout << "exit\n" << std::flush;
    }
    _out_bin_file.put(0x2);
    if (_out_bin_file.bad()) {
        throw CodeGenError(
                "Error:CodeGenVisitor::visit_script(): Bad file in code "
                "generation");
    }
    // Exit codes: 0 - no errors, close the application
    //             1 - no errors, go to main menu
    //             ... errors
    _out_bin_file.put(0x1);
    if (_out_bin_file.bad()) {
        throw CodeGenError(
                "Error:CodeGenVisitor::visit_script(): Bad file in code "
                "generation");
    }
}

void CodeGenVisitor::visit_script(const ScriptAst* mn_script_ptr)
{
    if (_verb) {
        std::cout << "script\n" << std::flush;
    }
    // write starting instructions
    _out_bin_file.put(0x01);
    if (_out_bin_file.bad()) {
        throw CodeGenError(
                "Error:CodeGenVisitor::visit_script(): Bad file in code "
                "generation");
    }
    int32_t start_pos = 0x5;
    _out_bin_file.write(reinterpret_cast<char*>(&start_pos), sizeof(int32_t));
    if (_out_bin_file.bad()) {
        throw CodeGenError(
                "Error:CodeGenVisitor::visit_script(): Bad file in code "
                "generation");
    }

    // iterate over AST
    const StmtAst* ptrCurStmt = mn_script_ptr->stmt.get();
    ptrCurStmt->accept(*this);

    while (ptrCurStmt->next_stmt != nullptr) {
        ptrCurStmt = ptrCurStmt->next_stmt.get();
        ptrCurStmt->accept(*this);
    }
}

void CodeGenVisitor::visit_stmt(const StmtAst* mn_stmt_ptr)
{
    if (mn_stmt_ptr->expr == nullptr) {
        throw CodeGenError(
                "Error:CodeGenVisitor::visit_stmt():  nullptr as StmtAst.expr, "
                "wrong structure of AST");
    }
    mn_stmt_ptr->expr->accept(*this);
}

void CodeGenVisitor::visit_persona(const PersonaAst* mn_persona_ptr)
{
    // add to Env (if there is a persona with the same name: new link in Env)
    Persona& persona = _env_reference.add_persona(mn_persona_ptr->p_id);

    const InsideAst* ptrCurInside = mn_persona_ptr->inside.get();
    visit_ins_type(ptrCurInside->memb_type.get(), persona);

    while (ptrCurInside->next != nullptr) {
        ptrCurInside = ptrCurInside->next.get();
        visit_ins_type(ptrCurInside->memb_type.get(), persona);
    }
}

void CodeGenVisitor::visit_ins_type(
        const InsTypeAst* mn_ins_type_ptr, Persona& persona)
{
    using st = ScriptToken;

    switch (mn_ins_type_ptr->lexem.token) {
        case st::PATH:
            persona.set_path(mn_ins_type_ptr->str_param);
            return;

        case st::VAR:
            persona.add_fg_var(
                    mn_ins_type_ptr->lexem.id, mn_ins_type_ptr->str_param);
            return;

        case st::NAME:
            persona.set_name(mn_ins_type_ptr->str_param);
            return;

        case st::FG:
            persona.set_main_fg(mn_ins_type_ptr->str_param);
            return;

        default:
            throw SemanticError(
                    "Not PATH, VAR, NAME or FG token as Persona member, check "
                    "your SemanticAnalyzer for bugs, dude;");
    }
}

void CodeGenVisitor::visit_persona_var(const PersonaVarAst* mn_p_var_ptr)
{
    // all checks about files are considered to be done in semantic analysis
    // persona must be initialized
    if (!_env_reference.add_info_to(
                mn_p_var_ptr->p_id, mn_p_var_ptr->memb_id, mn_p_var_ptr->param))
    {
        throw SemanticError("Error: persona " + mn_p_var_ptr->p_id
                            + " wasn't initialized;");
    }
}

void CodeGenVisitor::visit_bg_file(const BackFileAst* mn_bg_ptr)
{
    if (_verb) {
        std::cout << "load bg\t" << mn_bg_ptr->path_bg << std::endl;
    }
    // request file for later copying
    fs::path file_path(mn_bg_ptr->path_bg);
    _res_paths.emplace(file_path);

    file_path = file_path.filename();
    std::string temp_file_path = file_path.string();
    const char* ptrPathBg = temp_file_path.c_str();
    _out_bin_file.put(0x10);  // instruction code
    if (_out_bin_file.bad()) {
        throw CodeGenError(
                "Error:CodeGenVisitor::visit_script(): Bad file in code "
                "generation");
    }
    _out_bin_file.write(
            ptrPathBg, static_cast<int64_t>(std::strlen(ptrPathBg) + 1));
    if (_out_bin_file.bad()) {
        throw CodeGenError(
                "Error:CodeGenVisitor::visit_script(): Bad file in code "
                "generation");
    }
    // every path must be 64 chars long: all empty space is zeroed
    int64_t pos_diff = 64 - static_cast<int64_t>(std::strlen(ptrPathBg));
    while (pos_diff-- > 1) {
        _out_bin_file.put(0x0);
    }
}

void CodeGenVisitor::visit_fg_file(const ForeFileAst* mn_fg_f_ptr)
{
    if (_verb) {
        std::cout << "load fg\t" << mn_fg_f_ptr->path_fg << std::endl;
    }
    // request file for later copying
    fs::path file_path(mn_fg_f_ptr->path_fg);
    _res_paths.emplace(file_path);

    /// TODO: use "add fg" in pair with "load fg"
    // instruction code: for now it's "add fg", not "load fg", to simplify
    file_path = file_path.filename();
    std::string temp_file_path = file_path.string();
    const char* ptrPathFg = temp_file_path.c_str();
    _out_bin_file.put(0x20);
    if (_out_bin_file.bad()) {
        throw CodeGenError(
                "Error:CodeGenVisitor::visit_script(): Bad file in code "
                "generation");
    }
    _out_bin_file.write(
            ptrPathFg, static_cast<int64_t>(std::strlen(ptrPathFg) + 1));
    if (_out_bin_file.bad()) {
        throw CodeGenError(
                "Error:CodeGenVisitor::visit_script(): Bad file in code "
                "generation");
    }
    // every path must be 64 chars long -- all empty space is zeroed
    int64_t pos_diff = 64 - static_cast<int64_t>(std::strlen(ptrPathFg));
    while (pos_diff-- > 1) {
        _out_bin_file.put(0x0);
    }
}

void CodeGenVisitor::visit_fg_persona(const ForePersonaAst* fg_pers_ptr)
{
    if (_verb) {
        std::cout << "load fg\t" << fg_pers_ptr->memb_name << std::endl;
    }
    // request file for later copying
    std::string p_memb_fg_path;
    if (fg_pers_ptr->memb_name == "foreground") {
        p_memb_fg_path =
                _env_reference.get_persona(fg_pers_ptr->p_id).get_main_fg();
    } else {
        p_memb_fg_path = _env_reference.get_persona(fg_pers_ptr->p_id)
                                 .get_fg(fg_pers_ptr->memb_name);
    }
    fs::path file_path(p_memb_fg_path);
    _res_paths.emplace(file_path);

    /// TODO: use "add fg" in pair with "load fg"
    // instruction code: for now it's "add fg", not "load fg", to simplify
    file_path = file_path.filename();
    std::string temp_file_path = file_path.string();
    const char* ptrPathFg = temp_file_path.c_str();
    _out_bin_file.put(0x22);
    if (_out_bin_file.bad()) {
        throw CodeGenError(
                "Error:CodeGenVisitor::visit_script(): Bad file in code "
                "generation");
    }
    _out_bin_file.write(
            ptrPathFg, static_cast<int64_t>(std::strlen(ptrPathFg) + 1));
    if (_out_bin_file.bad()) {
        throw CodeGenError(
                "Error:CodeGenVisitor::visit_script(): Bad file in code "
                "generation");
    }
    // every path must be 64 chars long -- all empty space is zeroed
    int64_t pos_diff = 64 - static_cast<int64_t>(std::strlen(ptrPathFg));
    while (pos_diff-- > 1) {
        _out_bin_file.put(0x0);
    }
}

/// TODO:Q: adding another file for texts
void CodeGenVisitor::visit_txt_line(const TextLineAst* txt_line_ptr)
{
    if (_verb) {
        std::cout << "load textline\n" << std::flush;
    }
    if (txt_line_ptr->text.empty()) {
        return;
    }
    // 1. For now simply break line into 64 bytes long lines and fully load it
    // into the code
    // 2. substr_utf8_min() returns right till size() if len is bigger
    // in this case we don't use write(..., 64)
    const int32_t instl = 64;  // instruction length

    std::string sub_text_64 = substr_utf8_min(txt_line_ptr->text, 0, instl - 1);
    for (size_t chpos = 0; !sub_text_64.empty();) {
        const char* ptrChSubText = sub_text_64.c_str();
        _out_bin_file.put(0x30);
        if (_out_bin_file.bad()) {
            throw CodeGenError(
                    "Error:CodeGenVisitor::visit_script(): Bad file in code "
                    "generation");
        }
        _out_bin_file.write(ptrChSubText,
                static_cast<int64_t>(std::strlen(ptrChSubText) + 1));
        if (_out_bin_file.bad()) {
            throw CodeGenError(
                    "Error:CodeGenVisitor::visit_script(): Bad file in code "
                    "generation");
        }
        // every path must be 64 chars long -- all empty space is zeroed
        int32_t pos_diff = instl - static_cast<int32_t>(sub_text_64.size());
        while (pos_diff-- > 1) {
            _out_bin_file.put(0x0);
            if (_out_bin_file.bad()) {
                throw CodeGenError(
                        "Error:CodeGenVisitor::visit_script(): Bad file in "
                        "code "
                        "generation");
            }
        }
        chpos += sub_text_64.size();
        sub_text_64 = substr_utf8_min(txt_line_ptr->text, chpos, instl - 1);
    }
}

void CodeGenVisitor::visit_txt_file(const TextFileAst*)
{
    /// TODO: copy contents from .txt right into code
}

[[nodiscard]] const std::set<fs::path>& CodeGenVisitor::get_copyset() const
{
    return _res_paths;
}

}  // namespace vino