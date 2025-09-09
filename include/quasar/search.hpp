// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <cstddef>
#include <string>
#include <string_view>
#include <vector>

namespace quasar::search {

// Find all occurrences of 'needle' in 'haystack' using the Two-Way algorithm.
std::vector<size_t> find_all(std::string_view haystack, std::string_view needle);

// Find all occurrences using Boyer–Moore–Horspool (useful for longer needles).
std::vector<size_t> find_all_bmh(std::string_view haystack, std::string_view needle);

// Optional SIMD prefilter (runtime-detected); always verifies scalar
std::vector<size_t> find_all_simd_prefilter(std::string_view haystack, std::string_view needle);

} // namespace quasar::search