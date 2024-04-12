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
 * @return `std::string` substring
 * @note Doesn't throw error if char is invalid, it's implied that substring
 * just starts within the middle of multibyte UNICODE symbol, so that symbol is
 * skipped
 */
inline std::string substr_utf8_min(
        const std::string &str, size_t pos, size_t len) noexcept
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
                continue;
                // throw std::runtime_error("Invalid UTF-8 sequence");
            }
        } else {
            substr += c;
        }
    }
    return substr;
}

/**
 * @brief Little sugar for converting basic_strings
 *
 * @param string const std::basic_string<char_type_1>
 * @return std::basic_string<char_type_2>
 */
template <typename _Ch1, typename _Ch2>
inline std::basic_string<_Ch2> convert_str(
        const std::basic_string<_Ch1> &string)
{
    return {string.cbegin(), string.cend()};
}

// clang-format off

/**
 * @brief Converts a UTF-8 encoded string to a UTF-32 encoded string.
 *
 * @param utf8str The input UTF-8 string to convert
 * @return The resulting UTF-32 encoded string
 */
inline std::u32string to_utf32str(const std::string &utf8str) noexcept
{
    std::u32string u32str;
    for (std::size_t i = 0; i < utf8str.size(); i++) {
        char ch = utf8str[i];
        if ((ch & 0b10000000) != 0) {
            if ((0b01100000 & ch) == 0b01000000) {
                if (i + 1 < utf8str.size()) {
                    u32str += (0UL
                        | (static_cast<uint32_t>(
                                0x1F & static_cast<uint8_t>(ch)
                            ) << 6)
                        | static_cast<uint32_t>(
                                0x3F & static_cast<uint8_t>(utf8str[++i])));
                } else {
                    // throw
                    return u32str;
                }
            } else if ((0b01100000 & ch) == 0b01100000) {
                if ((0b00010000 & ch) != 0) {
                    if (i + 3 < utf8str.size()) {
                        u32str += (0UL 
                            | (static_cast<uint32_t>(
                                    0x03 & static_cast<uint8_t>(ch)
                                ) << 18)
                            | (static_cast<uint32_t>(
                                    0x3F & static_cast<uint8_t>(utf8str[i + 1])
                                ) << 12)
                            | (static_cast<uint32_t>(
                                    0x3F & static_cast<uint8_t>(utf8str[i + 2])
                                ) << 6)
                            | (0x3F & static_cast<uint8_t>(utf8str[i + 3])));
                        i += 3;
                    } else {
                        return u32str;
                    }
                } else {
                    if (i + 2 < utf8str.size()) {
                        u32str += (0UL 
                            | (static_cast<uint32_t>(
                                    0x0F & static_cast<uint8_t>(ch)
                                ) << 12)
                            | (static_cast<uint32_t>(
                                    0x3F & static_cast<uint8_t>(utf8str[i + 1])
                                ) << 6)
                            | (0x3F & static_cast<uint8_t>(utf8str[i + 2])));
                        i += 2;
                    } else {
                        return u32str;
                    }
                }
            } else {
                continue;
                // throw std::runtime_error("Invalid UTF-8 sequence");
            }
        } else {
            u32str += (0UL | static_cast<uint8_t>(ch));
        }
    }
    return u32str;
}
// clang-format on

}  // namespace vino
