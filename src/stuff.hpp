#pragma once

#include <algorithm>
#include <string>

namespace vino {

bool insen_str_equal(const std::string &lhs, const std::string &rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(),
                      [](unsigned char l, unsigned char r) -> bool {
                          return std::tolower(l) == std::tolower(r);
                      });
}

}  // namespace vino
