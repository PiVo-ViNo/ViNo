#include "CodeGen.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>

#include "VisitorImplementations.hpp"

namespace vino {

/// TODO: user defined copydir settings
void CodeGen::run(ScriptAst* in_ast, std::ostream& out_stream, bool verbose)
{
    _verb = verbose;
    CodeGenVisitor gen_code_visitor(out_stream, _env, _verb);
    gen_code_visitor.visit_script(in_ast);
    // copy_resources("./finalVN/res", gen_code_visitor.get_copyset());
    _verb = false;
}

void CodeGen::copy_resources(const fs::path&           copydir,
                             const std::set<fs::path>& copyset) const
{
    if (copyset.empty()) {
        std::cout << "Warning: copyset is empty" << std::endl;
        return;
    }
    fs::create_directories(copydir);
    if (!fs::exists(copydir)) {
        throw std::runtime_error("Copy directory couldn't be created");
    }
    for (const auto& file : copyset) {
        std::cout << file << std::endl;
        fs::copy_file(file, copydir / file.filename(),
                      fs::copy_options::overwrite_existing);
    }
}

}  // namespace vino