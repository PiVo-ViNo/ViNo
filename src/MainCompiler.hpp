#pragma once

#include <filesystem>

namespace m_comp {

namespace fs = std::filesystem;

struct ArgsFlags {
    bool script_path_set;
    bool dev_mod_set;
    bool test_set;
};

bool path_integrity(const char *path) noexcept;

fs::path check_args(int loc_argc, const char **loc_argv, ArgsFlags &args_flags);

/**
    @brief Main compilation run function
*/
void compilation_main(int loc_argc, const char **loc_argv);

}  // namespace m_comp