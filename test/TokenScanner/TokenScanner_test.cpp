#include <exception>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <filesystem>

#include "TokenEnum.h"
#include "custom_errors.h"
#include "boost/ut.hpp"
#include "TokenScanner.h"

int main() {

    using namespace boost::ut;
    using namespace boost::ut::literals;
    using namespace boost::ut::operators::terse;

    using vst = vino::ScriptToken;
    using vpt = vino::PairTokenId;

    const std::string input_test = 
    "#comment\n"
    "background = \"bg.png\"\n"
    "#comment\n"
    "persona Human {path=\"./human\", background=\"bg.png\",\n"
    "foreground = \"human0.png\", name = \"Humanio\"}\n"
    "exit\n";

    const std::vector<vst> output_test = {
        vst::NEW_LINE,

        vst::BG, vst::SIGN_EQ, vst::TEXT_LINE,
        vst::NEW_LINE,

        vst::NEW_LINE,

        vst::PERSONA, vst::VAR, vst::BRACE_OP, vst::PATH,
        vst::SIGN_EQ, vst::TEXT_LINE, vst::COMMA,
        vst::BG, vst::SIGN_EQ, vst::TEXT_LINE, 
        vst::COMMA, vst::NEW_LINE, 
        
        vst::FG, vst::SIGN_EQ, vst::TEXT_LINE,
        vst::COMMA, vst::NAME, vst::SIGN_EQ, vst::TEXT_LINE,
        vst::BRACE_CL,
        vst::NEW_LINE,

        vst::EXIT 
    };

    "tokenizer_test0"_test = [&output_test, &input_test] {
        std::vector<vst> output_tokenizer;
        expect(nothrow([&input_test, &output_tokenizer] {
            std::string input = input_test;
            vino::TokenScanner tokenizer(std::move(input));
            output_tokenizer = tokenizer.get_raw_tokens(true);
        } )); 
        expect(output_tokenizer == output_test);
    };
    
    return 0;
}
