#include "stuff.hpp"

#include "boost/ut.hpp"
#include <filesystem>
#include <string>
#include <fstream>

namespace fs = std::filesystem;

int main()
{
    using namespace boost::ut;
    using namespace boost::ut::literals;
    using namespace boost::ut::operators::terse;

    "utf8str_to_utf32str"_test = [] {
        std::string utf8str = "Я плыву in the 海";
        std::u32string utf32str = U"Я плыву in the 海";
        expect(vino::to_utf32str(utf8str) == utf32str);
    };

    "exact_substr_utf8"_test = [] {
        // Cyrillic characters takes up 2 bytes in UTF-8
        std::string utf8str = "Абракадабра";
        expect(vino::substr_utf8_min(utf8str, 0, 8) == "Абра");
    };

    "partial_substr_utf8"_test = [] {
        // Cyrillic characters takes up 2 bytes in UTF-8
        std::string utf8str = "Абракадабра";
        expect(vino::substr_utf8_min(utf8str, 0, 9) == "Абра");
    };

    "middle_substr_utf8"_test = [] {
        // Cyrillic characters takes up 2 bytes in UTF-8
        std::string utf8str = "Абракадабра";
        // [2, 7)
        expect(vino::substr_utf8_min(utf8str, 2, 5) == "бр");
    };

    "middle_partial_substr_utf8"_test = [] {
        // Cyrillic characters takes up 2 bytes in UTF-8
        std::string utf8str = "Абракадабра";
        // [3, 7)
        expect(vino::substr_utf8_min(utf8str, 3, 4) == "р");
    };
    return 0;
}