#pragma once

#include <algorithm>
#include <string>

namespace vino {

/**
 * @brief Case insensitive strings comparison
 *
 * @remark Unlike C-style strcmp(), it doesn't return int
 * @param lhs const std::basic_string<_Ch>&
 * @param rhs const std::basic_string<_Ch>&
 * @return bool
 */
template <typename _Ch>
inline bool insen_str_equal(const std::basic_string<_Ch> &lhs,
        const std::basic_string<_Ch>                     &rhs) noexcept
{
    if (lhs.size() != rhs.size()) return false;
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend(),
            [](const _Ch &l, const _Ch &r) -> bool {
                return std::tolower(static_cast<int>(l))
                       == std::tolower(static_cast<int>(r));
            });
}

/**
 * @brief Get the smallest utf-8 sensitive substring
 *
 * @details Get the biggest substring within boundaries [pos, pos + len). If
 * unicode symbol is encoded with, e.g. 3 bytes, but substring has only 2 bytes
 * free space, this symbol won't be included in the resulting substring.
 * @param pos Starting position in `str`
 * @param len Right boundary of how many chars (1 byte) can be included
 * @throw `std::runtime_error` in case there is problem with utf-8 encoding
 * @return `std::string` substring
 */
inline std::string substr_utf8_min(
        const std::string &str, size_t pos, size_t len)
{
    if (len == 0 || pos >= str.size()) {
        return "";
    }
    if (pos + len > str.size()) {
        len = str.size() - pos;
    }
    std::string substr;
    for (size_t i = pos; i < pos + len && i < str.size(); i++) {
        char c = str[i];
        if ((c & 0b10000000) != 0) {
            if ((0b01100000 & c) == 0b01000000) {
                if (i + 1 < str.size() && i + 1 < pos + len) {
                    substr += str.substr(i, 2);
                }
                i++;
            } else if ((0b01100000 & c) == 0b01100000) {
                if ((0b00010000 & c) != 0) {
                    if (i + 3 < str.size() && i + 3 < pos + len) {
                        substr += str.substr(i, 4);
                    }
                    i += 3;
                } else {
                    if (i + 2 < str.size() && i + 2 < pos + len) {
                        substr += str.substr(i, 3);
                    }
                    i += 2;
                }
            } else {
                throw std::runtime_error("Invalid UTF-8 sequence");
            }
        } else {
            substr += c;
        }
    }
    return substr;
}

/**
 * @brief Convert std::string to std::u32string
 * 
 * @param u8string 
 * @return std::u32string 
 */
inline std::u32string to_u32string(const std::string& u8string)
{
    return {u8string.cbegin(), u8string.cend()};
}

}  // namespace vino
