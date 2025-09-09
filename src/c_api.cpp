// SPDX-License-Identifier: Apache-2.0
#include "quasar/c_api.h"
#include "quasar/search.hpp"
#include <string>
#include <string_view>

extern "C" size_t quasar_count_substring(const char* haystack, size_t hay_len,
                                         const char* needle,   size_t needle_len) {
  if (!haystack || !needle) return 0;
  std::string_view hay(haystack, hay_len);
  std::string_view nee(needle, needle_len);
  auto matches = quasar::search::find_all(std::string(hay), std::string(nee));
  return matches.size();
}
