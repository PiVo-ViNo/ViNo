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
    explicit CodeGen(bool verbal) : _verb(verbal) {}

    /// TODO: Consider it making noexcept
    const std::set<fs::path> run(
            ScriptAst* in_ast, std::ostream& out_stream, bool verbose = false);

    [[nodiscard]] bool copy_resources(
            const fs::path& copydir, const std::set<fs::path>& copyset) const;

private:
    SymbolTableEnv _env{};
    bool           _verb = false;
};

}  // namespace vino