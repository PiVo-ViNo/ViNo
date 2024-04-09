#pragma once

#include <filesystem>

namespace m_comp {

namespace fs = std::filesystem;

struct ArgsFlags {
    bool unknown_set = false;
    bool script_path_set = false;
    bool verbose_mode_set = false;
    bool test_set = false;
    bool help_run_set = false;
};

bool path_integrity(const char *path) noexcept;

fs::path check_args(int loc_argc, const char **loc_argv, ArgsFlags &args_flags);

/**
 * @brief Main compilation run function
 */
void compilation_main(
        int loc_argc, const char **loc_argv, bool throw_err = false);

}  // namespace m_comp