/*
 * main.cpp
 *
 * Created on Mon Feb 26 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#include <bitset>
#include <cstring>
#include <filesystem>
#include <iostream>

const char *_f_path = "--path";
const char *_f_;

bool path_integrity(char *path) {
    namespace fs = std::filesystem;

    fs::path _src_path{path};
    if (fs::exists(_src_path) && _src_path.extension() == ".vnsf") return true;

    return false;
}

int main(int argc, char **argv) {
    /* Flags for existence of arguments
     *  0:  --path      / -p
     *  1:  --dev-mode  / -d
     *  2:  --test      / -t
     */
    std::bitset<3> _flags{};

    std::filesystem::path script_path{};

    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            std::cout << argv[i] << '\n';
            if (argc - i >= 2 && !std::strcmp(argv[i], _f_path) &&
                path_integrity(argv[i + 1])) {
                _flags.set(0);
                script_path = argv[i + 1];
            } else if (false) {
            } else {
                std::cout << "ERROR : Unknown argument " << argv[i]
                          << std::endl;
                return -1;
            }
        }
    } else {  // needs a check if there is the running instance of program,
              //  if there are many, give error "Many instances : cannot attach
              //  to random"
        std::cout << "Program needs a source script file to run" << std::endl;
        return 1;
    }
    return 0;
}