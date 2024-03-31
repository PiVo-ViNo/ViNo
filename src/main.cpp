/*
 * main.cpp
 *
 * Created on Mon Feb 26 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */

#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>

#include "Parser.hpp"
#include "SemanticAnalyzer.hpp"
#include "SymbolTable.hpp"
#include "TokenEnum.hpp"
#include "TokenScanner.hpp"
#include "custom_errors.hpp"

namespace fs = std::filesystem;

namespace m_exe {

const char *arg_path = "--path";
const char *short_arg_path = "-p";

struct ArgsFlags {
    bool script_path_set;
    bool dev_mod_set;
    bool test_set;
};

bool path_integrity(char *path) noexcept
{
    namespace fs = std::filesystem;

    fs::path src_path(path);
    if (fs::exists(src_path) && src_path.extension() == ".vnsf") {
        return true;
    }
    return false;
}

fs::path check_args(int argc, char **argv, ArgsFlags &args_flags)
{
    fs::path script_path;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (argc - i >= 1
                && !(std::strcmp(argv[i], m_exe::arg_path)
                     && std::strcmp(argv[i], m_exe::short_arg_path)))
            {
                if (!m_exe::path_integrity(argv[++i])) {
                    throw vino::ArgsError("ERROR: Script file error in --path");
                }
                args_flags.script_path_set = true;
                script_path = argv[i];
            } else {
                throw vino::ArgsError("ERROR: Unknown argument: ");
            }
        }
    } else {  // needs a check if there is the running instance of program,
              //  if there are many, give error "Many instances : cannot attach
              //  to random"
        throw vino::ArgsError("Program needs a source script file to run");
    }
    return script_path;
}

}  // namespace m_exe

int main(int argc, char **argv)
{
    /* Flags for existence of arguments
     *  0:  --path      / -p
     *  1:  --dev-mode  / -d
     *  2:  --test      / -t
     */
    m_exe::ArgsFlags      main_args;
    std::filesystem::path script_path;

    try {
        script_path = m_exe::check_args(argc, argv, main_args);
    } catch (vino::ArgsError &err) {
        std::cout << err.what() << std::endl;
        return 1;
    }

    using vpair = vino::PairTokenId;

    std::ifstream main_script{script_path, std::ios::binary};

    vino::TokenScanner main_tok_scanner(std::move(main_script));

    std::function<vpair()> f_get_tokens = [&main_tok_scanner] {
        return main_tok_scanner.get_token();
    };
    vino::Parser    main_parser(f_get_tokens);
    vino::ScriptAst main_ast(main_parser.run());
    std::cout << "Successful parsing\n" << std::flush;

    vino::SymbolTableEnv   main_symb_env;
    vino::SemanticAnalyzer main_sem_anal(main_symb_env, main_ast);
    main_sem_anal.run_analysis();
    std::cout << "Successful semantic analysis\n" << std::flush;

    return 0;
}
