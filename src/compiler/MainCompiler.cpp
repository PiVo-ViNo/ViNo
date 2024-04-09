/*
 * Compiler.cpp
 *
 * Created on Sun Mar 31 2024
 *
 * (c) Andrjusha (aka SibJusha)
 *
 */
#include "MainCompiler.hpp"

#include "AST.hpp"
#include "CodeGen.hpp"
#include "Parser.hpp"
#include "SemanticAnalyzer.hpp"
#include "SymbolTable.hpp"
#include "TokenEnum.hpp"
#include "TokenScanner.hpp"

#include "custom_errors.hpp"
#include <filesystem>
#include <fstream>
#include <ios>
#include <iostream>
#include <ostream>
#include <stdexcept>

namespace m_comp {

const std::string help = "--help";
const std::string short_help = "-h";
const std::string arg_verbose = "--verbose";
const std::string short_arg_verbose = "-v";

bool path_integrity(const char *path) noexcept
{
    // std::cout << fs::current_path() << std::endl;
    fs::path src_path(path);
    if (fs::exists(src_path) && src_path.extension().string() == ".vnsf") {
        return true;
    }
    return false;
}

fs::path check_args(int loc_argc, const char **loc_argv, ArgsFlags &args_flags)
{
    fs::path script_path;
    if (loc_argc > 1) {
        for (int i = 1; i < loc_argc; i++) {
            if (std::string(loc_argv[i]) == m_comp::arg_verbose
                    || std::string(loc_argv[i]) == m_comp::short_arg_verbose)
            {
                args_flags.verbose_mode_set = true;
            } else if (std::string(loc_argv[i]) == m_comp::help
                       || std::string(loc_argv[i]) == m_comp::short_help)
            {
                args_flags.help_run_set = true;
            } else if (m_comp::path_integrity(loc_argv[i])) {
                args_flags.script_path_set = true;
                script_path = loc_argv[i];
            } else {
                args_flags.unknown_set = true;
                args_flags.help_run_set = true;
            }
        }
    }
    return script_path;
}

void compilation_main(int loc_argc, const char **loc_argv, bool throw_err)
{
    using vpair = vino::PairTokenId;

    /* Flags for existence of arguments
     *  1:  --verbose   / -v
     *  2:  --test      / -t
     *  3:  --help      / -h
     */
    m_comp::ArgsFlags main_args;
    fs::path          script_path;

    // Read command line options and write if need
    script_path = m_comp::check_args(loc_argc, loc_argv, main_args);
    if (!main_args.help_run_set && !main_args.script_path_set) {
        if (throw_err) {
            throw vino::ArgsError(
                    "ERROR: Program needs a source script file to compile");
        }
        std::cout << "ERROR: Program needs a source script file to compile\n";
    }
    if (main_args.unknown_set) {
        if (throw_err) {
            throw vino::ArgsError("ERROR: Uknown option");
        }
        std::cout << "ERROR: Unknown option\n";
    }
    if (main_args.help_run_set || !main_args.script_path_set) {
        std::cout << "ViNo Complier v0.1.0\n"
                     "Usage: ViNoCompiler [options] <source_file>.vnsf\n\n"
                     "Options:\n"
                     "\t-h, --help : run this help message\n"
                     "\t-v, --verbose : output more compiler processes\n"
                  << std::flush;
        return;
    }
    if (main_args.verbose_mode_set) {
        std::cout << "Running verbose mode\n" << std::flush;
    }

    // Try to open script file
    std::ifstream main_script{script_path, std::ios::in | std::ios::binary};
    if (!main_script.good()) {
        std::runtime_error("ERROR: Bad file " + script_path.string());
    }

    vino::TokenScanner     main_tok_scanner(std::move(main_script));
    std::function<vpair()> f_get_tokens = [&main_tok_scanner] {
        return main_tok_scanner.get_token();
    };

    vino::Parser    main_parser(f_get_tokens);
    vino::ScriptAst main_ast = main_parser.run(main_args.verbose_mode_set);
    std::cout << "Successful parsing\n" << std::flush;

    vino::SymbolTableEnv   main_symb_env;
    vino::SemanticAnalyzer main_sem_anal(main_symb_env, main_ast);
    main_sem_anal.run(main_args.verbose_mode_set);
    std::cout << "Successful semantic analysis\n" << std::flush;

    vino::CodeGen main_code_gen;
    fs::create_directory("./finalVN");
    std::fstream out_fbin(
            "./finalVN/m_vm_inst_" + script_path.stem().string() + ".bin",
            std::ios::trunc | std::ios::out | std::ios::binary);
    main_code_gen.run(&main_ast, out_fbin, main_args.verbose_mode_set);
    std::cout << "Successful code generation\n" << std::flush;
}

}  // namespace m_comp