// SPDX-License-Identifier: Apache-2.0
#include "quasar/search.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  std::string hay(reinterpret_cast<const char*>(data), size);
  // Simple seeds for patterns
  static const char* needles[] = {"a","test","quasar","\n","\0"};
  for (auto n : needles) {
    auto v = quasar::search::find_all(hay, n);
    // Touch results to avoid optimization-out
    if (!v.empty() && v[0] > hay.size()) return 0;
  }
  return 0;
}
