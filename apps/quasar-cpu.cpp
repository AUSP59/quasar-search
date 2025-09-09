// SPDX-License-Identifier: Apache-2.0
#include <iostream>
#include <thread>
#include "quasar/cpu.hpp"

int main() {
  unsigned n = std::thread::hardware_concurrency();
  auto f = quasar::cpu::detect();
  std::cout << "hardware_concurrency=" << (n ? n : 1)
            << " sse2=" << f.sse2
            << " sse4.2=" << f.sse42
            << " avx2=" << f.avx2 << "\n";
  return 0;
}