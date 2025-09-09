// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <algorithm>
#include <cstdint>
#include <vector>

namespace quasar::metrics {

inline double percentile(std::vector<double> v, double p) {
  if (v.empty()) return 0.0;
  if (p <= 0) return *std::min_element(v.begin(), v.end());
  if (p >= 100) return *std::max_element(v.begin(), v.end());
  std::sort(v.begin(), v.end());
  double idx = (p/100.0) * (v.size() - 1);
  size_t lo = static_cast<size_t>(idx);
  size_t hi = std::min(lo + 1, v.size() - 1);
  double frac = idx - lo;
  return v[lo] * (1.0 - frac) + v[hi] * frac;
}

} // namespace quasar::metrics