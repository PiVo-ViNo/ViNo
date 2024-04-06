#include "MainCompiler.hpp"

#include "boost/ut.hpp"
#include "custom_errors.hpp"
#include <filesystem>
#include <stdexcept>

namespace fs = std::filesystem;

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::literals;
    using namespace boost::ut::operators::terse;


    test("positive_basic") = [] {
        expect(nothrow([] {
            const char *ptr_argv_loc[] = {"", "--path",
                                          "./resources/test.vnsf"};
            m_comp::compilation_main(3, ptr_argv_loc);
        }));
    };

    "negative_no_persona"_test = [] {
        expect(throws<vino::SemanticError>([] {
            const char *ptr_argv_loc[] = {"", "--path",
                                          "./resources/short.vnsf"};
            m_comp::compilation_main(3, ptr_argv_loc);
        }));
    };

    "positive_long"_test = [] {
        expect(nothrow([] {
            const char *ptr_argv_loc[] = {"", "--path",
                                          "./resources/long_copy.vnsf"};
            m_comp::compilation_main(3, ptr_argv_loc);
        }));
    };

    "negative_no_such_png"_test = [] {
        expect(throws<vino::SemanticError>([] {
            const char *ptr_argv_loc[] = {"", "--path",
                                          "./resources/missing.vnsf"};
            m_comp::compilation_main(3, ptr_argv_loc);
        }));
    };

    "negative_no_script_file"_test = [] {
        expect(throws<vino::ArgsError>([] {
            const char *ptr_argv_loc[] = {"", "--path",
                                          "./resources/non_exist.vnsf"};
            m_comp::compilation_main(3, ptr_argv_loc);
        }));
    };

    return 0;
}
