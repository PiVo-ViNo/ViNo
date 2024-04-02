#pragma once

#include "AST.hpp"
#include "SymbolTable.hpp"

#include <filesystem>
#include <ostream>
#include <set>

namespace vino {

namespace fs = std::filesystem;

class CodeGen {
public:
    CodeGen() = default;

    /// TODO: Consider it making noexcept
    void run(ScriptAst* in_ast, std::ostream& out_stream, bool verbose = false);

private:
    SymbolTableEnv _env{};
    bool           _verb = false;

    void copy_resources(const fs::path&           copydir,
                        const std::set<fs::path>& copyset) const;
};

}  // namespace vino