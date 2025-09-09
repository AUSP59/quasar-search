// SPDX-License-Identifier: Apache-2.0
#pragma once
#include <cstdint>

namespace quasar::cpu {

struct Features {
  bool sse2 = false;
  bool sse42 = false;
  bool avx2 = false;
};

Features detect();

} // namespace quasar::cpu