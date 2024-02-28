#include "TokenScanner.h"

int main() {

    // -------- Test Constructors ------------

    TokenScanner tokenizer0{};

    std::ifstream _ifstream;
    TokenScanner tokenizer1(std::move(_ifstream));
    
    std::string _istr;
    TokenScanner tokenizer2(std::move(_istr));

    return 0;
}