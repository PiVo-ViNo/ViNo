#include <exception>
#include <vector>
#include <iostream>
#include <stdexcept>

#include "Parser.h"
#include "TokenEnum.h"
#include "custom_errors.h"
#include "ut.hpp"

//! IMPORTANT: create test.txt test.png and dir (directory) where testing binary is
//! or change parameters in TestTokenizer::test_get_token()
class TestTokenizer {
public:
    explicit TestTokenizer(const std::vector<vino::ScriptToken>& tokens) :
        tokens_vec(tokens)
    {
    }

    vino::PairTokenId test_get_token()
    {
        if (tokens_vec[pos] == vino::ScriptToken::PATH) {
            previous_is_path = true;
        } else if (tokens_vec[pos] == vino::ScriptToken::TEXT_TYPE) {
            previous_is_text_type = true;
        }
        if (previous_is_text_type && tokens_vec[pos] == vino::ScriptToken::TEXT_LINE) {
            previous_is_text_type = false;
            return vino::PairTokenId(tokens_vec[pos++], "./test.txt");
        }
        if (previous_is_path && tokens_vec[pos] == vino::ScriptToken::TEXT_LINE) {
            previous_is_path = false;
            return vino::PairTokenId(tokens_vec[pos++], "./dir/");
        }
        return vino::PairTokenId(tokens_vec[pos++], "./test.png");
    }

private:
    size_t pos = 0;
    bool previous_is_path = false;
    bool previous_is_text_type = false;

    const std::vector<vino::ScriptToken>& tokens_vec;
};

int main() {

    using namespace boost::ut;
    using namespace boost::ut::literals;
    using namespace boost::ut::operators::terse;

    typedef vino::ScriptToken vst;

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

        vst::PERSONA, vst::VAR, vst::BRACE_OP, vst::PATH,
        vst::SIGN_EQ, vst::TEXT_LINE, vst::COMMA,
        vst::BG, vst::SIGN_EQ, vst::TEXT_LINE,
        vst::COMMA, vst::FG, vst::SIGN_EQ, vst::TEXT_LINE,
        vst::COMMA, vst::NAME, vst::SIGN_EQ, vst::TEXT_LINE,
        vst::BRACE_CL,
        vst::NEW_LINE,

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

    "parser_empty"_test = [] {
        expect(throws<vino::ParsingError>([] { 
            vino::Parser parser;
            parser.run();
        } )); 
    };
    
    "parser_not_empty"_test = [&tokens_test] {
        expect(nothrow([&tokens_test] {
            vino::Parser parser;
            parser.set_input(tokens_test);
            parser.run(true);
        }));
    };

    "parser_input_set_on_init"_test = [&tokens_test] {
        expect(nothrow([&tokens_test] {
            vino::Parser parser(tokens_test);
            parser.run(true);
        }));
    };

    "parser_input_texts"_test = [&tokens_text] {
        expect(nothrow([&tokens_text] {
            vino::Parser parser(tokens_text);
            parser.run(true);
        }));
    };
 
    "parser_exit_only"_test = [&tokens_exit] {
        expect(nothrow([&tokens_exit] {
            vino::Parser parser(tokens_exit);
            parser.run(true);
        }));
    };   

    "parser_persona_only"_test = [&WRONGtokens_0] {
        expect(throws<vino::ParsingError>([&WRONGtokens_0] { 
            vino::Parser parser(WRONGtokens_0);
            parser.run();
        } )); 
    };
    
    "parser_unclosed_persona"_test = [&WRONGtokens_1] {
        expect(throws<vino::ParsingError>([&WRONGtokens_1] { 
            vino::Parser parser(WRONGtokens_1);
            parser.run();
        } )); 
    };   
    
    "parser_no_tokens"_test = [&tokens_empty] {
        expect(throws<vino::ParsingError>([&tokens_empty] { 
            vino::Parser parser(tokens_empty);
            parser.run();
        } )); 
    };   
    
    return 0;
}
