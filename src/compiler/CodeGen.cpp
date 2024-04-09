#include "CodeGen.hpp"
#include "VisitorImplementations.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace vino {

/// TODO: user defined copydir settings
void CodeGen::run(ScriptAst* in_ast, std::ostream& out_stream, bool verbose)
{
    _verb = verbose;
    CodeGenVisitor gen_code_visitor(out_stream, _env, _verb);
    gen_code_visitor.visit_script(in_ast);
    copy_resources("./finalVN/res", gen_code_visitor.get_copyset());
    _verb = false;
}

void CodeGen::copy_resources(
        const fs::path& copydir, const std::set<fs::path>& copyset) const
{
    if (copyset.empty()) {
        std::cout << "Warning: copyset is empty" << std::endl;
        return;
    }
    fs::create_directories(copydir);
    if (!fs::exists(copydir)) {
        throw std::runtime_error("Copy directory couldn't be created");
    }
    bool success = true;
    for (const auto& file : copyset) {
        if (fs::copy_file(file, copydir / file.filename(),
                fs::copy_options::overwrite_existing))
        {
            std::cout << "Successfully copied " << file << "\n";
        } else {
            success = false;
            std::cout << "Cannot copy " << file << "\n";
        }
    }
    if (!success) {
        std::cout << "Copy is unsuccessful, removing successfully copied";
        for (const auto& file : copyset) {
            // no need to check if exists, it happens inside
            fs::remove(copydir / file.filename());
        } 
    }
    std::cout << std::flush;
}

}  // namespace vino