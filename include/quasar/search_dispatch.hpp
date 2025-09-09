// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <string_view>
#include <vector>

namespace quasar::search {

// Choose the best strategy based on needle size and platform hints.
std::vector<size_t> find_all_best(std::string_view haystack, std::string_view needle, bool simd_prefilter);

} // namespace quasar::search