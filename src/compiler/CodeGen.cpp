#include "CodeGen.hpp"
#include "VisitorImplementations.hpp"
#include <filesystem>
#include <iostream>

namespace vino {

const std::set<fs::path> CodeGen::run(
        ScriptAst* in_ast, std::ostream& out_stream, bool verbose)
{
    _verb = verbose;
    CodeGenVisitor gen_code_visitor(out_stream, _env, _verb);
    gen_code_visitor.visit_script(in_ast);
    // copy_resources("./finalVN/res", gen_code_visitor.get_copyset());
    _verb = false;
    return gen_code_visitor.get_copyset();
}

bool CodeGen::copy_resources(
        const fs::path& copydir, const std::set<fs::path>& copyset) const
{    
    if (copyset.empty()) {
        std::cout << "Warning: copyset is empty" << std::endl;
        return false;
    }
    fs::create_directories(copydir);
    if (!fs::exists(copydir)) {
        std::cout << "Error: output directory couldn't be created" << std::endl;
        return false;
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
            break;
        }
    }
    if (!success) {
        std::cout << "Copy is unsuccessful, removing successfully copied";
        std::cout << std::endl;
        fs::remove_all(copydir);
        return false;
    }
    std::cout << std::flush;
    return true;
}

}  // namespace vino