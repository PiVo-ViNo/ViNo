#include "CodeGen.hpp"

#include "boost/ut.hpp"
#include <iostream>

int main() {
    using namespace boost::ut;
    using namespace boost::ut::literals;
    using namespace boost::ut::operators::terse;

    "positive_basic"_test = [] {
        expect(nothrow([] {
            vino::CodeGen code_generator;
            code_generator.run(, std::ostream &out_stream)
                                          "./resources/test.vnsf"};
        }));
    };

    return 0;
}