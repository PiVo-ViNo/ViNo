#include "Parser.hpp"

#include "SemanticAnalyzer.hpp"
#include "TokenEnum.hpp"
#include "boost/ut.hpp"
#include "custom_errors.hpp"

#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;

namespace test_utils {

class Tester {
    fs::path tmp{std::filesystem::temp_directory_path() / "test_parser"};

public:
    explicit Tester(const std::vector<vino::ScriptToken>& tokens) :
        tokens_vec(tokens)
    {
        fs::create_directory(tmp);
        fs::create_directory(tmp / "dir");
        std::ofstream ostrm(tmp / "test.png", std::ios::binary | std::ios::out);
        ostrm.put('a');
        ostrm.open(tmp / "test.txt", std::ios::binary | std::ios::out);
        ostrm.put('a');
    }

    vino::PairTokenId test_get_token()
    {
        if (pos >= tokens_vec.size()) {
            return {vino::ScriptToken::EXIT, ""};
        }
        if (tokens_vec[pos] == vino::ScriptToken::PATH) {
            previous_is_path = true;
        } else if (tokens_vec[pos] == vino::ScriptToken::TEXT_TYPE) {
            previous_is_text_type = true;
        }
        if (previous_is_text_type
            && tokens_vec[pos] == vino::ScriptToken::TEXT_LINE)
        {
            previous_is_text_type = false;
            return {tokens_vec[pos++], tmp.string() + "/test.txt"};
        }
        if (previous_is_path && tokens_vec[pos] == vino::ScriptToken::TEXT_LINE)
        {
            previous_is_path = false;
            return {tokens_vec[pos++], tmp.string() + "/dir"};
        }
        return {tokens_vec[pos++], tmp.string() + "/test.png"};
    }

    ~Tester() { fs::remove_all(tmp); }

private:
    size_t pos = 0;
    bool   previous_is_path = false;
    bool   previous_is_text_type = false;

    const std::vector<vino::ScriptToken>& tokens_vec;
};

}  // namespace test_utils

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::literals;
    using namespace boost::ut::operators::terse;

    using vst = vino::ScriptToken;
    using vpt = vino::PairTokenId;

    // clang-format off
    /* Input:
     * #comment
     * background = "bg.png"
     * #comment
     * persona Human {path="./human", background="bg.png",
     * foreground = "human0.png", name = "Humanio"}
     * exit
     */
    const std::vector<vst> tokens_test = {
        vst::NEW_LINE,

        vst::BG, vst::SIGN_EQ, vst::TEXT_LINE,
        vst::NEW_LINE,

        vst::NEW_LINE,

        vst::PERSONA, vst::VAR, vst::BRACE_OP, 
        vst::PATH, vst::SIGN_EQ, vst::TEXT_LINE, vst::COMMA,
        vst::NAME, vst::SIGN_EQ, vst::TEXT_LINE, vst::COMMA, 
        vst::FG, vst::SIGN_EQ, vst::TEXT_LINE, vst::COMMA, 
        vst::VAR, vst::SIGN_EQ, vst::TEXT_LINE,
        vst::BRACE_CL,
        vst::NEW_LINE,

        vst::TEXT_TYPE, vst::TEXT_LINE, vst::NEW_LINE,

        vst::TEXT_TYPE, vst::SIGN_EQ, vst::TEXT_LINE, vst::NEW_LINE,

        vst::EXIT 
    };

    /* Input:
     * text = "text.txt"
     * text "text" 
    */
    const std::vector<vst> tokens_text = {
        vst::TEXT_TYPE, vst::SIGN_EQ, vst::TEXT_LINE,
        vst::NEW_LINE,

        vst::TEXT_TYPE, vst::TEXT_LINE,
        vst::NEW_LINE,

        vst::EXIT
    };

    const std::vector<vst> tokens_exit = {
        vst::EXIT
    };

    const std::vector<vst> WRONGtokens_0 = {
        vst::PERSONA,

        vst::EXIT
    };

    const std::vector<vst> WRONGtokens_1 = {
        vst::PERSONA, vst::VAR, vst::BRACE_OP,
        vst::NEW_LINE,

        vst::EXIT
    };

    const std::vector<vst> tokens_empty = {};

    // clang-format on

    "parser_empty"_test = [&tokens_empty] {
        expect(nothrow([&tokens_empty] {
            test_utils::Tester tokenizer(tokens_empty);
            
            std::function<vino::PairTokenId()> fn = [ptrObject = &tokenizer] {
                return ptrObject->test_get_token();
            };

            vino::Parser parser(fn);
            parser.run(true);
        }));
    };

    "parser_not_empty"_test = [&tokens_test] {
        expect(nothrow([&tokens_test] {
            test_utils::Tester   tokenizer(tokens_test);

            std::function<vino::PairTokenId()> fn = [ptrObject = &tokenizer] {
                return ptrObject->test_get_token();
            };

            vino::Parser parser(fn);
            parser.run(true);
        }));
    };

    "parser_input_set_on_init"_test = [&tokens_text] {
        expect(nothrow([&tokens_text] {
            test_utils::Tester   tokenizer(tokens_text);

            std::function<vino::PairTokenId()> fn = [ptrObject = &tokenizer] {
                return ptrObject->test_get_token();
            };

            vino::Parser parser(fn);
            parser.run(true);
        }));
    };

    "parser_input_texts"_test = [&tokens_exit] {
        expect(nothrow([&tokens_exit] {
            test_utils::Tester   tokenizer(tokens_exit);

            std::function<vino::PairTokenId()> fn = [ptrObject = &tokenizer] {
                return ptrObject->test_get_token();
            };

            vino::Parser parser(fn);
            parser.run(true);
        }));
    };

    "parser_persona_only"_test = [&WRONGtokens_0] {
        expect(throws<vino::ParsingError>([&WRONGtokens_0] {
            test_utils::Tester   tokenizer(WRONGtokens_0);

            std::function<vino::PairTokenId()> fn = [ptrObject = &tokenizer] {
                return ptrObject->test_get_token();
            };

            vino::Parser parser(fn);
            parser.run();
        }));
    };

    "parser_unclosed_persona"_test = [&WRONGtokens_1] {
        expect(throws<vino::ParsingError>([&WRONGtokens_1] {
            test_utils::Tester   tokenizer(WRONGtokens_1);

            std::function<vino::PairTokenId()> fn = [ptrObject = &tokenizer] {
                return ptrObject->test_get_token();
            };

            vino::Parser parser(fn);
            parser.run();
        }));
    };

    "parser_no_tokens"_test = [&tokens_empty] {
        expect(nothrow([&tokens_empty] {
            test_utils::Tester   tokenizer(tokens_empty);

            std::function<vino::PairTokenId()> fn = [ptrObject = &tokenizer] {
                return ptrObject->test_get_token();
            };

            vino::Parser parser(fn);
            parser.run();
        }));
    };

    return 0;
}
