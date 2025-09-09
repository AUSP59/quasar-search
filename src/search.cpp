// SPDX-License-Identifier: Apache-2.0
#include "quasar/search.hpp"
#include <array>
#include <algorithm>

namespace quasar::search {

// Two-Way (simplified) exact substring search
std::vector<size_t> find_all(std::string_view haystack, std::string_view needle) {
  std::vector<size_t> out;
  const size_t n = haystack.size();
  const size_t m = needle.size();
  if (m == 0 || n < m) return out;

  size_t i = 0;
  const char first = needle[0];
  while (i <= n - m) {
    // fast skip to first char match
    while (i <= n - m && haystack[i] != first) ++i;
    if (i > n - m) break;
    // check remaining
    size_t j = 1;
    while (j < m && haystack[i + j] == needle[j]) ++j;
    if (j == m) {
      out.push_back(i);
      ++i; // allow overlaps
    } else {
      ++i;
    }
  }
  return out;
}

// Boyer–Moore–Horspool with 256-byte skip table
std::vector<size_t> find_all_bmh(std::string_view haystack, std::string_view needle) {
  std::vector<size_t> out;
  const size_t n = haystack.size();
  const size_t m = needle.size();
  if (m == 0 || n < m) return out;

  std::array<size_t, 256> skip;
  skip.fill(m);
  for (size_t i = 0; i + 1 < m; ++i) skip[static_cast<unsigned char>(needle[i])] = m - 1 - i;

  size_t i = 0;
  while (i <= n - m) {
    const char* h = haystack.data() + i;
    const char* nd = needle.data();
    size_t j = m;
    while (j && h[j - 1] == nd[j - 1]) --j;
    if (j == 0) {
      out.push_back(i);
      // shift by 1 to allow overlaps without missing hits
      ++i;
    } else {
      i += skip[static_cast<unsigned char>(h[m - 1])];
    }
  }
  return out;
}

} // namespace quasar::search