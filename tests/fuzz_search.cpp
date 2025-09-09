// SPDX-License-Identifier: Apache-2.0
#include "quasar/search.hpp"
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (size < 2) return 0;
  size_t half = size / 2;
  std::string hay(reinterpret_cast<const char*>(data), half);
  std::string needle(reinterpret_cast<const char*>(data + half), size - half);
  auto a = quasar::search::find_all(hay, needle);
  auto b = quasar::search::find_all_bmh(hay, needle);
  if (a != b) __builtin_trap();
  return 0;
}