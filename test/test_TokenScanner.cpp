#define BOOST_TEST_MODULE boost_test_macro_overview
#include <boost/test/included/unit_test.hpp>

#include "TokenScanner.h"

#include <vector>

BOOST_AUTO_TEST_CASE( test_macro_overview )
{
    namespace tt = boost::test_tools;
    
    std::string _istr_test = "#comment\n"
                        "background ash = \"ash.png\n\""
                        "foreground Oleg = \"olegus.jpeg\"\n"
                        "persona Chad {foreground = \"Chad.png\"}\n"
                        "text \"Hello!\"\n"
                        "text = \"hello.txt\"\n";

    typedef vino::ScriptToken vst;

    const std::vector<vst> tokens_test = {
        vst::BG, vst::VAR, vst::SIGN_EQ, vst::TEXT_LINE, vst::NEW_LINE,

        vst::FG, vst::VAR, vst::SIGN_EQ, vst::TEXT_LINE, vst::NEW_LINE,

        vst::PERSONA, vst::VAR, vst::BRACE_OP, vst::FG, vst::SIGN_EQ, 
        vst::TEXT_LINE, vst::BRACE_CL, vst::NEW_LINE,

        vst::TEXT_TYPE, vst::TEXT_LINE, vst::NEW_LINE,

        vst::TEXT_TYPE, vst::SIGN_EQ, vst::TEXT_LINE, vst::NEW_LINE
    };

    vino::TokenScanner tokenizer(std::move(_istr_test));
    std::vector<vst> tokens_get = tokenizer.get_all_tokens(); 
    BOOST_TEST(tokens_get == tokens_test);
}

/* int main() {

    // -------- Test Constructors ------------

    vino::TokenScanner tokenizer0{};

    std::ifstream _ifstream;
    vino::TokenScanner tokenizer1(std::move(_ifstream));
    
    std::string _istr;
    vino::TokenScanner tokenizer2(std::move(_istr));

    return 0;
} */