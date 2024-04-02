#include "TokenScanner.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "TokenEnum.hpp"
#include "boost/ut.hpp"

namespace fs = std::filesystem;

namespace test_utils {

class Tester {
public:
    explicit Tester(std::string str_tokens)
    {
        fs::create_directory(tmp);
        std::ofstream ostrm(tmp / "test.txt", std::ios::binary);
        ostrm << str_tokens;
        ostrm.close();
    }

    ~Tester() { fs::remove_all(tmp); }

    [[nodiscard]] fs::path get_txt_path() const { return tmp / "test.txt"; }

private:
    fs::path tmp{fs::temp_directory_path() / "test_tokenizer"};
};

}  // namespace test_utils

int main()
{
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
        "foreground = \"human0.png\", name = \"Humanio\",\n"
        "angry = \"./angry.png\"}\n"
        "foreground = Human\n"
        "foreground = Human.angry\n"
        "exit\n";

    const std::vector<vst> output_test = {
        vst::NEW_LINE,

        vst::BG,       vst::SIGN_EQ,   vst::TEXT_LINE, vst::NEW_LINE,

        vst::NEW_LINE,

        vst::PERSONA,  vst::VAR,       vst::BRACE_OP,  vst::PATH,
        vst::SIGN_EQ,  vst::TEXT_LINE, vst::COMMA,     vst::BG,
        vst::SIGN_EQ,  vst::TEXT_LINE, vst::COMMA,     vst::NEW_LINE,
        vst::FG,       vst::SIGN_EQ,   vst::TEXT_LINE, vst::COMMA,
        vst::NAME,     vst::SIGN_EQ,   vst::TEXT_LINE, vst::COMMA, 
        vst::NEW_LINE,
        vst::VAR,      vst::SIGN_EQ,   vst::TEXT_LINE, vst::BRACE_CL, 
        vst::NEW_LINE,

        vst::FG, vst::SIGN_EQ, vst::VAR, vst::NEW_LINE,

        vst::FG, vst::SIGN_EQ, vst::VAR, vst::DOT, vst::VAR,
        vst::NEW_LINE, 

        vst::EXIT};

    "tokenizer_test0"_test = [&output_test, &input_test] {
        std::vector<vst> output_tokenizer;
        expect(nothrow([&input_test, &output_tokenizer] {
            std::string        input = input_test;
            vino::TokenScanner tokenizer(std::move(input));
            output_tokenizer = tokenizer.get_raw_tokens(true);
        }));
        expect(output_tokenizer == output_test);
    };

    "tokenizer_test1_file"_test = [&output_test, &input_test] {
        std::vector<vst> output_tokenizer;
        expect(nothrow([&input_test, &output_tokenizer] {
            test_utils::Tester tester(input_test);
            std::ifstream      ifstrm(tester.get_txt_path(), std::ios::binary);
            vino::TokenScanner tokenizer(std::move(ifstrm));
            output_tokenizer = tokenizer.get_raw_tokens(true);
        }));
        expect(output_tokenizer == output_test);
    };

    return 0;
}
