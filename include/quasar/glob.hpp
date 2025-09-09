// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <string_view>

namespace quasar::glob {

inline bool match_one(char p, char s) {
  return p == '?' || p == s;
}

inline bool match(std::string_view pattern, std::string_view str) {
  size_t pi = 0, si = 0, star = std::string_view::npos, match_i = 0;
  while (si < str.size()) {
    if (pi < pattern.size() && (pattern[pi] == '?' || pattern[pi] == str[si])) {
      ++pi; ++si;
    } else if (pi < pattern.size() && pattern[pi] == '*') {
      star = pi++;
      match_i = si;
    } else if (star != std::string_view::npos) {
      pi = star + 1;
      si = ++match_i;
    } else {
      return false;
    }
  }
  while (pi < pattern.size() && pattern[pi] == '*') ++pi;
  return pi == pattern.size();
}

} // namespace quasar::glob