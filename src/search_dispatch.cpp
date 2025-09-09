// SPDX-License-Identifier: Apache-2.0
#include "quasar/search_dispatch.hpp"
#include "quasar/search.hpp"

namespace quasar::search {

std::vector<size_t> find_all_best(std::string_view haystack, std::string_view needle, bool simd_prefilter) {
  const size_t m = needle.size();
  if (m == 0) return {};
  if (simd_prefilter && m >= 2) {
    return find_all_simd_prefilter(haystack, needle);
  }
  // Heuristic: BMH tends to shine for m>=4; Two-Way for tiny needles.
  if (m >= 4) return find_all_bmh(haystack, needle);
  return find_all(haystack, needle);
}

} // namespace quasar::search