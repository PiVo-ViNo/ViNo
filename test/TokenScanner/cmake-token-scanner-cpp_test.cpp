#include <vector>
#include <iostream>
#include <cassert>

#include "TokenScanner.h"

// must be redone with boost.ut

int main() {

    // -------- Test Constructors ------------
    {
    vino::TokenScanner tokenizer0{};

    std::ifstream _ifstream;
    vino::TokenScanner tokenizer1(std::move(_ifstream));
    
    std::string _istr;
    vino::TokenScanner tokenizer2(std::move(_istr));
    }

    // ---------- Test Tokenizing ----------
    // -------------------------------------

    // ----------- Test 0 ------------------
    try {
    std::string _istr_test = "#comment\n"
                        "background ash = \"ash.png\"\n"
                        "foreground Oleg = \"olegus.jpeg\"\n"
                        "persona Chad {foreground = \"Chad.png\"}\n"
                        "text \"Hello!\"\n"
                        "text = \"hello.txt\"\n";

    typedef vino::ScriptToken vst;

    const std::vector<vst> tokens_test = {
        vst::NEW_LINE,

        vst::BG, vst::VAR, vst::SIGN_EQ, vst::TEXT_LINE, vst::NEW_LINE,

        vst::FG, vst::VAR, vst::SIGN_EQ, vst::TEXT_LINE, vst::NEW_LINE,

        vst::PERSONA, vst::VAR, vst::BRACE_OP, vst::FG, vst::SIGN_EQ, 
        vst::TEXT_LINE, vst::BRACE_CL, vst::NEW_LINE,

        vst::TEXT_TYPE, vst::TEXT_LINE, vst::NEW_LINE,

        vst::TEXT_TYPE, vst::SIGN_EQ, vst::TEXT_LINE, vst::NEW_LINE,

        vst::EXIT
    };

    vino::TokenScanner tokenizer(std::move(_istr_test));
    std::vector<vst> tokens_get = tokenizer.get_all_tokens(); 
    for (int i = 0; i < tokens_get.size(); i++) {
        std::cout << tokens_get[i] << '\t';
        std::cout << tokens_test[i] << '\n';
    }
    assert(tokens_get == tokens_test);
    } catch (std::exception& excp) {
        std::cout << excp.what() << '\n';
    }
    // ------------------ Test 1 ------------------------
    try {
    std::cout << '\n';
    std::string _istr_test = "background ash = \"ash.png\"\n";

    typedef vino::ScriptToken vst;
    std::vector<vst> tokens_test = {
        vst::BG, vst::VAR, vst::SIGN_EQ, vst::TEXT_LINE, vst::NEW_LINE,

        vst::EXIT
    };
    vino::TokenScanner tokenizer(std::move(_istr_test));
    std::vector<vino::ScriptToken> tokens_get = 
                                    tokenizer.get_all_tokens(true);
    assert(tokens_get == tokens_test);
    } catch (std::exception& excp) {
        std::cout << excp.what() << '\n';
    }
    return 0;
}