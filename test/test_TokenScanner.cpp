#include "TokenScanner.h"

int main() {

    // -------- Test Constructors ------------

    vino::TokenScanner tokenizer0{};

    std::ifstream _ifstream;
    vino::TokenScanner tokenizer1(std::move(_ifstream));
    
    std::string _istr;
    vino::TokenScanner tokenizer2(std::move(_istr));

    return 0;
}